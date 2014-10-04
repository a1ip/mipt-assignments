import csv
import random
import os

def WriteToFile(file, values):
  with open(file, 'wb') as csvfile:
    writer = csv.writer(csvfile, delimiter=',')
    writer.writerow(values)

counter = 0

def MakeTest(values):
  global counter
  WriteToFile('tests/input' + str(counter) + '.csv', values)
  counter += 1

if not os.path.exists('tests'):
  os.makedirs('tests')

MakeTest([0])
MakeTest([0] * 10)
MakeTest([-1])
MakeTest([-1] * 11)
MakeTest([1])
MakeTest([1] * 11)

MakeTest(range(10, 100))
MakeTest(range(-100, -10))
MakeTest(range(-193, 193))

MakeTest([random.randint(-1023, -46) for i in xrange(1000)])
MakeTest([random.randint(23, 2046) for i in xrange(1000)])
MakeTest([random.randint(-1023, 2046) for i in xrange(1000)])
MakeTest([random.randint(-10 ** 6, 10 ** 6) for i in xrange(10 ** 5 * 2)])

MakeTest([random.randint(-1023, -46) for i in xrange(1000)])
MakeTest([random.randint(23, 2046) for i in xrange(1000)])
MakeTest([random.randint(-1023, 2046) for i in xrange(1000)])
MakeTest([random.randint(-10 ** 6, 10 ** 6) for i in xrange(10 ** 5 * 2)])
