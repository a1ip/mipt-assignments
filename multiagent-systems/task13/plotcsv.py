#!/usr/bin/python2

__author__ = 'misa'

import csv
import argparse
import matplotlib
matplotlib.use('Agg')

import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description='Plot description')
parser.add_argument('name', type=str, help='Name of csv file (output of BehaviorSpace) to plot the graphic')

args = parser.parse_args()
filename = args.name

values = []
years = 0

table = open(filename)
for row in table:
	row = row.split(",")
	if (not row[0]):
		values.append(float(row[1].strip('\n').strip('"')))
		years = years + 1

filename = filename.split('.')[0]

plt.plot(range(years), values)
plt.savefig(filename+'.png')

table.close()
