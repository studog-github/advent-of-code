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

def manhattan_dist(p1, p2):
    return abs(p1[0] - p2[0]) + abs(p1[1] - p2[1])
