import math
import random

import numpy as np
import matplotlib.pyplot as plt
import scipy.optimize as opt

time_left = 0.0
time_right = 1.0
time_steps = 10000
tau = (time_right - time_left) / time_steps

u0 = 0.0
du0 = 1.0
u1 = 0.0

lmb_min = 0.0
lmb_max = 20.0
lmb_steps = 20
#lmb_step = (lmb_max - lmb_min) / lmb_steps

def calculate_values(lmb):
  values = [u0, u0 + du0 * tau]
  for n in xrange(2, time_steps):
    values.append(values[n - 1] * (2 - tau ** 2 * (lmb - (n * tau) ** 2)) - values[n - 2])
  return values

def calculate_discrepancy(lmb):
  return calculate_values(lmb)[-1] - u1

def calculate_discrepancies():
  discrepancies = []
  for lmb in np.linspace(lmb_min, lmb_max, num=lmb_steps):
    discrepancies.append(calculate_discrepancy(lmb))
  return discrepancies

def plot_values(lmb):
  times = np.linspace(time_left, time_right, num=time_steps)
  values = calculate_values(lmb)
  plt.plot(times, values)

def plot_descrepancies():
  lmbds = np.linspace(lmb_min, lmb_max, num=lmb_steps)
  discrepancies = calculate_discrepancies()
  plt.plot(lmbds, discrepancies, 'ro')

def plot_zeros():
  global time_steps
  global tau

  m = 50
  dt = 1000
  zeros = []

  for i in xrange(m):
    zeros.append(opt.newton(calculate_discrepancy, 10.0, maxiter=50, tol=1.48e-08))
    time_steps += dt
    tau = (time_right - time_left) / time_steps

  plt.plot(np.linspace(1000.0, 1000.0 + dt * m, num=m), zeros)

zero = opt.newton(calculate_discrepancy, 10.0, maxiter=50, tol=1.0e-06)
print zero

figure = plt.gcf()

axes = figure.add_subplot(111)
axes.axhline(linewidth=3, color="b")

#plot_zeros()
#plot_values(zero)
plot_descrepancies()

plt.grid(True)
plt.xlabel(u'\u03BB')
plt.ylabel(u'F(\u03BB)')

figure.set_size_inches(6.0, 5.0)
plt.savefig('lambda.png', dpi = 100)

plt.show()
