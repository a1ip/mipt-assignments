#!/usr/bin/python

import sys

import bspline

def print_usage():
    print '{} <output_file> <order> <knot_list>'.format(sys.argv[0])
    print 'Example: {} fig.ps 3 0 0 1 2 3 4 5'.format(sys.argv[0])

try:
    filename = sys.argv[1]
    order = int(sys.argv[2])
    knots = map(int, sys.argv[3:])
    assert len(knots) >= order + 2
except:
    print_usage()
    exit(0)

basis = bspline.Bspline(knots, order)
basis.plot()
bspline.plt.savefig(filename)
