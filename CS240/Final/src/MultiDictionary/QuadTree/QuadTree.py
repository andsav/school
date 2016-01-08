from ...config import *
from ..Multi import Multi


class QuadTree(Multi):

    def __init__(self, array):
        self.min = self.max = (None, None)
        self.compute_square(array)

    def compute_square(self, array):
        minx = maxx = array[0][0]
        miny = maxy = array[0][1]

        for item in array:
            if item[0] < minx:
                minx = item[0]
            if item[1] < miny:
                miny = item[1]
            if item[0] > maxx:
                maxx = item[0]
            if item[1] > maxy:
                maxy = item[1]

        self.min = (minx, miny)
        self.max = (maxx, maxy)

    def insert(self, tup):
        pass

    def range_search(self, tup1, tup2):
        pass