#!/usr/bin/python

import csv

f = open('result.csv')
r = csv.reader(f)

rows = []
for row in r:
  rows.append(row)

f.close()

rows = rows[7:11]
rows = [row[1:] for row in rows]

lsa = [int(x) for x in rows[0]]
mst = [int(x) for x in rows[1]]
gsa = [int(x) for x in rows[2]]
time = [int(x) for x in rows[3]]

reps = 50
min_time = 10 ** 10
best_params = None

for i in xrange(len(lsa) / reps - 1):
  times = time[i * reps: (i + 1) * reps]
  avg_time = float(sum(times)) / reps
  if avg_time < min_time:
    min_time = avg_time
    best_params = (lsa[i * reps], gsa[i * reps], mst[i * reps])

print 'minimum-average-time:', min_time
print 'local-search-angle:', best_params[0]
print 'global-search-angle:', best_params[1]
print 'max-search-time:', best_params[2]
