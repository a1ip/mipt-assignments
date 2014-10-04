import math
import random

import numpy as np
import matplotlib.pyplot as plt

# P(X = k) = (lmb * t)^k / k! * exp(-lmb * t)
# P(X = 0) = exp(-lmb * t)
# P(X > 0) = 1 - exp(-lmb * t) = r = Eq(0, 1)
# t = -1.0 / lmb * ln(1 - r) = -1.0 / lmb * ln(r'), r' = Eq(0, 1)

max_time = 100.0
lmb = 1.0

step_list = []
time_list = []

current_time = 0.0
while current_time <= max_time:
    r = random.random()
    delta = -1.0 / lmb * math.log(r)
    current_time += delta
    step_list.append(len(step_list))
    time_list.append(current_time)

plt.xlabel('Time')
plt.ylabel('Events')

plt.step(time_list, step_list)
plt.savefig('image.png')
plt.show()
