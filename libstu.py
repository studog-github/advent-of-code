from copy import deepcopy
from functools import reduce
import math
import fractions

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

def rotate_coords(pos, deg, orig=[0, 0]):
    # Currently only handles increments of 90 degrees
    # Default rotation is counterclockwise
    x, y = pos[0], pos[1]
    ndeg = deg % 360
    ox, oy = orig[0], orig[1]

    if ndeg == 90:
        nx = -(y - oy) + ox
        ny =  (x - ox) + oy
    elif ndeg == 180:
        nx = 2 * ox - x
        ny = 2 * oy - y
    elif ndeg == 270:
        nx =  (y - oy) + ox
        ny = -(x - ox) + oy
    else:
        nx = x
        ny = y

    return [nx, ny]

# Inspired by this answer
# https://stackoverflow.com/a/4544699/1352761
class GrowingList(list):
    def __init__(self, *args, fill_value=None):
        list.__init__(self, *args)
        self.fill_value = fill_value

    def __verify_size(self, index):
        if isinstance(index, slice):
            max_index = index.stop - 1
        else:
            max_index = index
        if max_index >= len(self):
            self.extend([self.fill_value] * (max_index + 1 - len(self)))

    def __setitem__(self, index, value):
        self.__verify_size(index)
        list.__setitem__(self, index, value)

    def __getitem__(self, index):
        self.__verify_size(index)
        return list.__getitem__(self, index)

# Factorization from
# https://stackoverflow.com/a/6800214/1352761
def factors(n):
    return sorted(set(reduce(list.__add__, ([i, n//i] for i in range(1, int(n**0.5) + 1) if n % i == 0))))

# lcm from
# https://stackoverflow.com/a/51716940/1352761
def lcm(a, b):
    return a * b // fractions.gcd(a, b)

# Neighbour generator
def neighbours(x, y, minx=-math.inf, miny=-math.inf, maxx=math.inf, maxy=math.inf):
    nexty = y + 1
    prevy = y - 1
    nextx = x + 1
    prevx = x - 1
    if nexty < maxy:
        if prevx >= minx:
            yield prevx, nexty
        yield x, nexty
        if nextx < maxx:
            yield nextx, nexty
    if nextx < maxx:
        yield nextx, y
    if prevy >= miny:
        if nextx < maxx:
            yield nextx, prevy
        yield x, prevy
        if prevx >= minx:
            yield prevx, prevy
    if prevx >= minx:
        yield prevx, y
