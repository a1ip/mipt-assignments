import csv
import sys
import subprocess
import shutil

def ReadIntsFromCSV(file):
  result = []
  with open(file, 'rb') as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    for row in reader:
      for value in row:
        raw = value.strip()
        if raw != "":
          result.append(int(raw))
  return result

def RunTest(input_file, output_file):
  input_values = ReadIntsFromCSV(input_file)
  try:
    output_values = ReadIntsFromCSV(output_file)
  except:
    return False
  input_values.sort()
  return input_values == output_values

def main():
  if len(sys.argv) != 3:
    print 'Usage: %s <sorting program> <number of input files>' % sys.argv[0]
    print 'All input files should be named as \'inputN.csv\' and put into folder \'tests\'.'
    return

  program_name = sys.argv[1]
  tests_count = int(sys.argv[2])

  print 'Running ' + str(tests_count) + ' tests...'

  passed = 0
  failed = 0

  for i in xrange(tests_count):
    input = 'tests/input' + str(i) + '.csv'
    output = 'output.csv'

    process = subprocess.Popen([program_name, input, output], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    process.communicate()

    test_status = RunTest(input, output)
    if test_status == True:
      passed += 1
    else:
      failed += 1

    print 'Test #' + str(i) + ': ' + str(test_status)

  if failed > 0:
    print 'Tests FAILED: ' + str(failed)

  if passed > 0:
    print 'Tests PASSED: ' + str(passed)

if __name__ == '__main__':
  main()
