from ..Compression import Compression
import math


class RunLength(Compression):
    def __init__(self, text):
        Compression.__init__(self, text)

    def encode(self):
        self.bitstring = self.text[0]
        j = 0
        while j < self.text.__len__():
            i = j
            count = 0
            while j < self.text.__len__() and self.text[i] == self.text[j]:
                count += 1
                j += 1
            self.bitstring += "0" * int(math.floor(math.log(count, 2)))
            self.bitstring += bin(count)[2:]

    def decode(self):
        pass