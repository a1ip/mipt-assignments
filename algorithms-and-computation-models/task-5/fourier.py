a = [1, 0, 3, 2, 0, 0, 0, 0]
b = [3, 3, 0, 2, 0, 0, 0, 0]
n = 8

def transform(array, size):
  result = []
  for i in xrange(0, size):
      current = [0] * size
      for k in xrange(0, size):
          current[(i * k) % size] += array[k]
      result.append(current)
  return result

aw = transform(a, n)
bw = transform(b, n)

for x in aw:
    print x

print ''

for x in bw:
   print x

def multiply(alpha, beta, size):
    result = [0] * size
    for i in xrange(0, size):
        for k in xrange(0, size):
            result[(i + k) % size] += alpha[i] * beta[k]
    return result

cw = [multiply(aw[i], bw[i], n) for i in xrange(0, n)]

print ''

for x in cw:
   print x
