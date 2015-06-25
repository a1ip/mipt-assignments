import json
import subprocess
import time

from nameko.rpc import rpc, RpcProxy
from eventlet.timeout import with_timeout
from eventlet import sleep

with open('params.json') as f:
  params = json.loads(f.read())

TIMEOUT = params['timeout']
SERVICES = params['services']

def echo(s):
  subprocess.call(['echo', s])

class DictService(object):
  name = None

  storage = {}
  transactions = {}

  counter = 0
  # index = None

  # s0 = None
  # ...
  # sN = None

  def unique_id(self):
    self.counter += 1
    return self.counter * SERVICES + self.index

  @rpc
  def put(self, key, value):
    echo('put [{0}: {1}]'.format(key, value))
    tid = self.unique_id()

    other_services = map(lambda x: self.__dict__['s' + str(x)], range(SERVICES - 1))

    replies = []
    for service in other_services:
      reply = service.prepare.async(tid, key, value)
      assert reply != None
      replies.append(reply)

    get_all_results = lambda replies: map(lambda x: x.result(), replies)
#    t1 = time.time()
    rvs = with_timeout(TIMEOUT, get_all_results, replies, timeout_value=None)
#    t2 = time.time()
#    echo('! ' + str(t2 - t1))

    if rvs == None:
      echo('Timeout!')
      for service in other_services:
        rv = service.abort(tid)
        assert rv == True
      return False

    for service in other_services:
      rv = service.commit(tid)
      assert rv == True

    self.storage[key] = value

    return True

  @rpc
  def prepare(self, tid, key, value):
    self.transactions[tid] = (key, value)
    return True

  @rpc
  def commit(self, tid):
    key, value = self.transactions[tid]
    self.storage[key] = value
    return True

  @rpc
  def abort(self, tid):
    self.transactions.pop(tid, None)
    return True

  @rpc
  def get(self, key):
    echo('get [{0}]'.format(key))
    return self.storage.get(key)

def construct_service_class(index):
  src = """
class DictService{0}(DictService):
  name = "DictService{0}"
  index = {0}
""".format(index)
  counter = 0
  for i in xrange(SERVICES):
    if i != index:
      src += "  s{0} = RpcProxy('DictService{1}')\n".format(counter, i)
      counter += 1
  return src

for i in xrange(SERVICES):
  exec construct_service_class(i)
