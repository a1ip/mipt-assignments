import math
import random

import numpy as np
import matplotlib.pyplot as plt

def shake(value, error):
    return value - error + 2.0 * error * random.random()

def shake_right(value, error):
    return value + error * random.random()

def generate_random_grid(f, a, b, n, dx, df):
    h = (b - a) / n
    xs = [shake(h * i, dx) for i in xrange(n + 1)]
    fs = [shake(f(h * i), df) for i in xrange(n + 1)]
    return (xs, fs)

def generate_bad_grid(f, a, b, n, dx, df):
    h = (b - a) / n
    xs = []
    for i in xrange(n + 1):
        if f(h * i) < f(h * (i + 1)):
            xs.append(h * i - dx)
        else:
            xs.append(h * i + dx)
    fs = [f(h * i) + df for i in xrange(n + 1)]
    return (xs, fs)

def integrate(xs, fs, n):
    result = 0.0
    for i in xrange(n):
        result += (xs[i + 1] - xs[i]) * (fs[i] + fs[i + 1]) / 2.0
    return result

def calculate_error(xs, fs, n, res):
    integral = integrate(xs, fs, n)
    error = abs(res - integral)
    return error

def calculate_theoretical_error(a, b, n, dx, df, m1, m2):
    h = (b - a) / n
    d1 = df
    d2 = m1 * dx
    d3 = m2 * h ** 2 / 8.0
    return (d1 + d2 + d3) * (b - a)

n = 1000
a = 0.0
b = np.pi * 2.0
df = 0.0
m1 = 1.0
m2 = 1.0

h = (b - a) / n
print 'h =', h

dx_min = 0.0
dx_max = h / 3.0
dx_step = (dx_max - dx_min) / 100

f = lambda x: math.sin(x)
res = -math.cos(a) + math.cos(b)

generators = [(generate_random_grid, 'ro'), (generate_bad_grid, 'gD')]

for (generator, point_type) in generators:
    pxs = []
    pys = []
    for i in xrange(50):
        dx = dx_max * random.random()
        xs, fs = generator(f, a, b, n, dx, df)
        error = calculate_error(xs, fs, n, res)
        pxs.append(dx)
        pys.append(error)
    plt.plot(pxs, pys, point_type)

dx = np.arange(dx_min, dx_max, dx_step)
error = calculate_theoretical_error(a, b, n, dx, df, m1, m2)
plt.plot(dx, error)

plt.grid(True)
plt.xlabel(u'\u0394 x')
plt.ylabel(u'\u0394 I')

figure = plt.gcf()
figure.set_size_inches(6.0, 5.0)
plt.savefig('plot.png', dpi = 150)
