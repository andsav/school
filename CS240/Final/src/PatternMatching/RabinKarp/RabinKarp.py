from ..Matching import Matching


class RabinKarp(Matching):
    def __init__(self, needle, haystack):
        Matching.__init__(self, needle, haystack)
        self.hash_table = []
        self.compute_hashes()

    def h1(self, string):
        pass

    def compute_hashes(self):
        pass

    def find(self):
        pass