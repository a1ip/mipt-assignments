import math

def _order(value, base = 10):
    v = abs(float(value))
    b = abs(float(base))
    if v == 0.0:
        return 0
    elif v >= 1.0:
        result = 0
        while v >= b:
            v /= b
            result += 1
        return result
    else:
        result = 0
        while v < 1.0:
            v *= b
            result -= 1
        return result

def value(ve):
    return ve.value
    
def error(ve):
    return ve.error
    
def max(ve):
    return ve.value + ve.error
    
def min(ve):
    return ve.value - ve.error

def sigma(ve):
    return ve.error / ve.value
    
def trunc(ve, n = 2):
    order = _order(ve.error)
    result = valerr()
    result.value = round(ve.value * (10.0 ** (-order)), n - 1) * (10.0 ** order)
    result.error = round(ve.error * (10.0 ** (-order)), n - 1) * (10.0 ** order)
    return result

def log(ve):
    result = valerr()
    result.value = math.log(ve.value)
    result.error = result.value * sigma(ve)
    return result

def sqrt(ve):
    result = valerr()
    result.value = math.sqrt(ve.value)
    result.error = abs(result.value * 0.5 * sigma(ve))
    return result
    
class valerr:
    def __init__(self, value = 0.0, error = 0.0):
        self.value = float(value)
        self.error = float(error)
        
    def __str__(self):
        order = _order(self.value)
        value = self.value * (10.0 ** (-order))
        error = self.error * (10.0 ** (-order))
        if order == 0:
            return str(value) + ' +/- ' + str(error)
        else:
            return '(' + str(value) + ' +/- ' + str(error) + ') 10^' + str(order)
        
    def __abs__(self):
        self.value = abs(self.value)
        
    def __ne__(self, other):
        return (max(self) < min(other)) or (max(other) < min(self))
        
    def __eq__(self, other):
        return not(self.__ne__(other))
        
    def __lt__(self, other):
        return max(self) < min(other)
        
    def __gt__(self, other):
        return other.__lt__(self)
        
    def __le__(self, other):
        return self.__lt__(other) or self.__eq__(other)
        
    def __ge__(self, other):
        return other.__le__(self)
        
    def __neg__(self):
        result = valerr()
        result.value = -self.value
        result.error = self.error
        return result
        
    def __pos__(self):
        result = valerr()
        result.value = self.value
        result.error = self.error
        return result
        
    def __add__(self, other):
        if isinstance(other, valerr) == True:
            result = valerr()
            result.value = self.value + other.value
            result.error = self.error + other.error
            return result
        else:
            result = valerr()
            result.value = self.value + other
            result.error = self.error
            return result
            
    def __sub__(self, other):
        if isinstance(other, valerr) == True:
            result = valerr()
            result.value = self.value - other.value
            result.error = self.error + other.error
            return result
        else:
            result = valerr()
            result.value = self.value - other
            result.error = self.error
            return result
        
    def __mul__(self, other):
        if isinstance(other, valerr) == True:
            result = valerr()
            result.value = self.value * other.value
            result.error = abs(result.value * math.sqrt(sigma(self) ** 2 + sigma(other) ** 2))
            return result
        else:
            result = valerr()
            result.value = self.value * other
            result.error = self.error * abs(other)
            return result
        
    def __div__(self, other):
        if isinstance(other, valerr) == True:
            result = valerr()
            result.value = self.value / other.value
            result.error = abs(result.value * math.sqrt(sigma(self) ** 2 + sigma(other) ** 2))
            return result
        else:
            result = valerr()
            result.value = self.value / other
            result.error = abs(self.error / other)
            return result
            
    def __radd__(self, other):
        return self.__add__(other)
        
    def __rsub__(self, other):
        return (-self).__add__(other)
        
    def __rmul__(self, other):
        return self.__mul__(other)
        
    def __rdiv__(self, other):
        return valerr(other, 0.0).__div__(self)
        
    def __pow__(self, power):
        result = valerr()
        result.value = self.value ** power
        result.error = abs(result.value * power * sigma(self))
        return result

if __name__ == '__main__':
    x = valerr(10.0 / 3, 2.0 / 3)
    y = valerr(20.0 / 3, 1.0 / 3)
    z = valerr(20.0, 5.0)

    print 2 * x, trunc(2 * x)
    print y, trunc(y)
    print 1 / z, trunc(1 / z)
    print x + y, trunc(x + y)
    print x * y, trunc(x * y)
    print x ** -10, trunc(x ** -10)
    print trunc((x * y ** 2) / z), trunc(x * (y ** 2 / z))
