from copy import deepcopy

def rotate(l, n, d='r'):
    _len = len(l)
    if _len <= 1:
        return
    _n = n % _len
    if d == 'r':
        _n *= -1
    l[:] = l[_n:] + l[:_n]

def signeq(a, b):
    return abs(a + b) == abs(a) + abs(b)
