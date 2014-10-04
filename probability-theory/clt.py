import math
import random

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab

max_value = 100
size = 100
attempts = 10000

sums = [sum([np.random.randint(0, max_value + 1) for x in xrange(0, size)]) for x in xrange(0, attempts)]
plt.hist(sums, 50, normed = True)

expected_value = 1.0 * size * max_value / 2
variance = 1.0 * size * max_value ** 2 / 12
sigma = math.sqrt(variance)
#print expected_value, variance, sigma

x = np.arange(expected_value - 4 * sigma, expected_value + 4 * sigma, 1.0)
plt.plot(x, mlab.normpdf(x, expected_value, sigma), color = 'red')

plt.xlabel('Sum')
plt.ylabel('Probability')

plt.grid(True)
plt.savefig('image.png')
plt.show()
