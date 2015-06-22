from nameko.rpc import rpc, RpcProxy

import subprocess

def echo(s):
  subprocess.call(['echo', s])

class DictService(object):
  name = None

  storage = {}
  transactions = {}

  counter = 0
  flavor = None

  first = None
  second = None

  def unique_id(self):
    self.counter += 1
    return self.counter * 3 + self.flavor

  @rpc
  def put(self, key, value):
    echo('Put {0}: {1}'.format(key, value))
    tid = self.unique_id()

    # TODO: async, abort on timeout.
    first_rv = self.first.prepare(tid, key, value)
    second_rv = self.second.prepare(tid, key, value)
    assert first_rv == True
    assert second_rv == True

    first_rv = self.first.commit(tid)
    second_rv = self.second.commit(tid)
    assert first_rv == True
    assert second_rv == True

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

  # TODO: abort

  @rpc
  def get(self, key):
    echo('Get {0}'.format(key))
    return self.storage[key]

class DictServiceA(DictService):
  name = "DictServiceA"
  flavor = 0
  first = RpcProxy('DictServiceB')
  second = RpcProxy('DictServiceC')

class DictServiceB(DictService):
  name = "DictServiceB"
  flavor = 1
  first = RpcProxy('DictServiceC')
  second = RpcProxy('DictServiceA')

class DictServiceC(DictService):
  name = "DictServiceC"
  flavor = 2
  first = RpcProxy('DictServiceA')
  second = RpcProxy('DictServiceB')
