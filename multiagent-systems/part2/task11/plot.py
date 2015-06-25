#!/usr/bin/python

import matplotlib.pyplot as plt

d1 = """
2 0.030
3 0.042
4 0.053
6 0.073
8 0.086
10 0.108
12 0.136
14 0.157
16 0.177
""".strip().split()
d1x = map(int, d1[::2])
d1y = map(float, d1[1::2])

plt.plot(d1x, d1y)
plt.xlabel('number of services')
plt.ylabel('average put time')
plt.savefig('1.png')

plt.clf()
plt.cla()

# # #

d2 = """
0.01 0.263 
0.02 0.261
0.03 0.280
0.04 0.212
0.05 0.167
0.06 0.145
0.07 0.144
0.08 0.108
0.09 0.109
0.10 0.098
""".strip().split()
d2x = map(float, d2[::2])
d2y = map(float, d2[1::2])

plt.plot(d2x, d2y)
plt.xlabel('put timeout')
plt.ylabel('average put time')
plt.savefig('2.png')

plt.clf()
plt.cla()

# # #

d3 = """
4 0.33
6 0.29
8 0.30
10 0.32
12 0.43
14 0.41
16 0.47
""".strip().split()
d3x = map(int, d3[::2])
d3y = map(float, d3[1::2])

plt.plot(d3x, d3y)
plt.xlabel('number of services')
plt.ylabel('fails percentage')
plt.savefig('3.png')

plt.clf()
plt.cla()
