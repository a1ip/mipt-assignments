from math import *

# y = a x + b
def interpolate(x1, x2, f1, f2):
    assert x2 > x1
    a = (f2 - f1) / (x2 - x1)
    b = (x2 * f1 - x1 * f2) / (x2 - x1)
    return (a, b)

def calc_error(x1, x2, f1, f2, df, dx):
    assert x2 > x1
    assert x2 - x1 > 2 * dx
    return ((x2 - x1 + 2 * dx) * df + dx * abs(f1 - f2)) / (x2 - x1 - 2 * dx)
    h = x2 - x1
    return (abs(f1 - f2) * dx * (h + 2 * dx) + 4 * h * dx * df + h * h * df) / h / (h - 2 * dx)

def test(x1, x2, f1, f2, df, dx):
    error = calc_error(x1, x2, f1, f2, df, dx)
    for dx1 in [-dx, dx]:
        for dx2 in [-dx, dx]:
            for df1 in [-df, df]:
                for df2 in [-df, df]:
                    (a, b) = interpolate(x1 + dx1, x2 + dx2, f1 + df1, f2 + df2)
                    #print abs(f1 - (a * x1 + b))
                    #print abs(f2 - (a * x2 + b))
                    if abs(f1 - (a * x1 + b)) > error:
                        return False
                    if abs(f2 - (a * x2 + b)) > error:
                        return False
                    
    return True
    
dx = 0.001
df = 0.01
x1 = 0.0
x2 = 1.0
f1 = 0.0
f2 = 1.0

assert test(x1, x2, f1, f2, df, dx) == True
    
print calc_error(x1, x2, f1, f2, df, dx)
print interpolate(x1 + dx, x2 - dx, f1 + df, f2 - df)
