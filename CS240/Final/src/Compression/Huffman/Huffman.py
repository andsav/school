from ..Compression import Compression
from...PriorityQueue.MinHeap import MinHeap
from...Trie import Trie

class Huffman(Compression):
    def __init__(self, text):
        Compression.__init__(self, text)
        self.frequencies, self.heap = { }, MinHeap.MinHeap(list())
        self.get_frequencies()

    def get_frequencies(self):
        pass

    def build_heap(self):
        pass

    def encode(self):
        while len(self.heap) >= 2:
            t1 = self.heap.delete_min()
            t2 = self.heap.delete_min()
            insert = Trie.Trie([t1, t2])
            self.heap.insert_key(self.frequencies[t1] + self.frequencies[t2], insert)

    def decode(self):
        pass