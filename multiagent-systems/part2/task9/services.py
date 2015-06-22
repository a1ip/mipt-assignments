from nameko.rpc import rpc, RpcProxy
from nameko.timer import timer

F = 1
S = 2
T = 1

class ServiceA(object):
  name = "ServiceA"
  values = {}

  @rpc
  def put(self, key, value):
    print 'Put {0}: {1}'.format(key, value)
    self.values[key] = value

  @rpc
  def get(self, key):
    print 'Get {0}'.format(key)
    return self.values[key]

class ServiceB(object):
  name = "ServiceB"
  key = F
  service_a = RpcProxy("ServiceA")

  @timer(interval=T)
  def send(self):
    self.service_a.put(ServiceB.key, ServiceB.key)
    ServiceB.key += S
