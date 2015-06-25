import json
import subprocess
import time

from nameko.rpc import rpc, RpcProxy
from nameko.timer import timer

with open('params.json') as f:
  params = json.loads(f.read())

TIMEOUT = params['timeout']

counter = 0
avg_time = 0.0
fails = 0

def echo(s):
  subprocess.call(['echo', s])

class PutterService(object):
  name = 'PutterService'
  s0 = RpcProxy('DictService0')
  s1 = RpcProxy('DictService1')

  @timer(interval=TIMEOUT)
  def test(self):
    global counter
    global avg_time
    global fails

    echo('Putting counter')
    t1 = time.time()
    rv = self.s0.put('counter', str(counter))
    t2 = time.time()
    if rv:
      echo('Success: ' + str(t2 - t1) + ' seconds')
    else:
      echo('Fail')
      fails += 1

    echo('Getting counter')
    rv = self.s1.get('counter')
    echo('Success: counter = ' + str(rv))

    counter += 1

    avg_time = (avg_time * (counter - 1) + (t2 - t1)) / counter
    echo('Average put time: ' + str(avg_time))
    echo('Fail percentage: ' + str(float(fails) / counter))
