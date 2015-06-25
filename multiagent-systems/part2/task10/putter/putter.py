from nameko.rpc import rpc, RpcProxy
from nameko.timer import timer

import subprocess

counter = 0

def echo(s):
  subprocess.call(['echo', s])

class PutterService(object):
  name = 'PutterService'
  dict_a = RpcProxy('DictService0')
  dict_b = RpcProxy('DictService1')

  @timer(interval=1)
  def test(self):
    global counter
    self.dict_a.put('counter', str(counter))
    echo('counter = ' + str(self.dict_b.get('counter')))
    counter += 1
