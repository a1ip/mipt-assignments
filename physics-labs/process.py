from math import *

from plot import *
from valerr import *

def calculator(data):
    x = data[0] + data[1]
    y = data[2] + data[3]
    xe = 1.0
    ye = 20.0
    return (x, y, xe, ye)
    
make_plot('input', 'output', 'x', 'y', calculator)

x = valerr(10.0 / 3, 2.0 / 3)
y = valerr(20.0 / 3, 1.0 / 3)
z = valerr(20.0, 5.0)

print 2 * x, trunc(2 * x)
print y, trunc(y)
print 1 / z, trunc(1 / z)
print x + y, trunc(x + y)
print x * y, trunc(x * y)
print x ** -10, trunc(x ** -10)
print trunc((x * y ** 2) / z), trunc(x * (y ** 2 / z))
