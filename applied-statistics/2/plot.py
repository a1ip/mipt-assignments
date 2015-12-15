#!/usr/bin/python

productivity = {}

with open('barley.txt', 'r+') as input:
  for line in input:
    line = line.split()
    if line[0] not in productivity.keys():
      productivity[line[0]] = {}
    line[2] = line[2].replace(',', '.')
    productivity[line[0]][line[1]] = float(line[2])

total_productivity = {}

for place in productivity.keys():
  for type in productivity[place].keys():
    if type not in total_productivity.keys():
      total_productivity[type] = 0.0
    total_productivity[type] += productivity[place][type]
    
print total_productivity
