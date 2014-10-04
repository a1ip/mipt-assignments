#!/usr/bin/python

import matplotlib.pyplot as plt
import math

x = []
u = []
v = []

with open('data') as data:
  for line in data:
    x.append(float(line.split(' ')[0]))
    u.append(float(line.split(' ')[1]))
    v.append(float(line.split(' ')[2]))

plt.plot(x, u, 'ro')
plt.plot(x, v, 'ro')

plt.title('Brusselator')
plt.xlabel('x, coordinate')
plt.ylabel('u, v, concentrations')

#plt.xlim([0.0, 5.0])
#plt.ylim([0.0, 3.0])

plt.show()
