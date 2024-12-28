from copy import deepcopy
from functools import reduce
import itertools
import math
import fractions
import heapq

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
def manhattan_delta(p1, p2):
    return p2[0] - p1[0], p2[1] - p1[1]

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

# p is a tuple with 2 or more dimensions
# limits is a list of boundary (min, max) for each dimension
def neighbours_a(p, limits=None, diagonals=False):
    for n in itertools.product(*[[i-1,i,i+1] for i in p]):
        # The < check makes neighbours() match range() in behaviour
        if limits is not None and False in [i[0]<=j<i[1] for i,j in zip(limits, n)]:
            continue
        if not diagonals and True not in [i==j for i,j in zip(p, n)]:
            continue
        if n == p:
            # p can't be its own neighbour
            continue
        yield n

# grid in list-of-rows format
def dijkstra(grid, start, end=None):
    maxx = len(grid[0])
    maxy = len(grid)
    limits = ((0,maxx),(0,maxy))

    distances = [[math.inf for x in range(maxx)] for y in range(maxy)]
    prev_node = [[None for x in range(maxx)] for y in range(maxy)]
    scanned = [[False for x in range(maxx)] for y in range(maxy)]

    sx,sy = start
    distances[sy][sx] = 0

    nodeq = [(0, start)]

    while len(nodeq) > 0:
        distance,node = heapq.heappop(nodeq)
        if end is not None and node == end:
            break
        nodex,nodey = node
        scanned[nodey][nodex] = True
        node_dist = distances[nodey][nodex]
        for n in neighbours_a(node, limits=limits):
            nx,ny = n
            if scanned[ny][nx]:
                continue
            n_dist = distances[ny][nx]
            new_n_dist = node_dist + grid[ny][nx]
            if new_n_dist < n_dist:
                distances[ny][nx] = new_n_dist
                prev_node[ny][nx] = node
                heapq.heappush(nodeq, (new_n_dist, n))
    return distances, prev_node

# lcm from
# https://stackoverflow.com/a/51716940/1352761
def lcm(a, b):
    return a * b // fractions.gcd(a, b)

# Neighbour generator
def neighbours_b(x, y, minx=-math.inf, miny=-math.inf, maxx=math.inf, maxy=math.inf, orthogonal=False):
    nexty = y + 1
    prevy = y - 1
    nextx = x + 1
    prevx = x - 1
    if nexty < maxy:
        if not orthogonal and prevx >= minx:
            yield prevx, nexty
        yield x, nexty
        if not orthogonal and nextx < maxx:
            yield nextx, nexty
    if nextx < maxx:
        yield nextx, y
    if prevy >= miny:
        if not orthogonal and nextx < maxx:
            yield nextx, prevy
        yield x, prevy
        if not orthogonal and prevx >= minx:
            yield prevx, prevy
    if prevx >= minx:
        yield prevx, y

# Dump a list of strings
def dump_map(m):
    for l in m:
        print(l)

# Dump a list of characters
def dump_map_join(m):
    for l in m:
        print(''.join(l))
