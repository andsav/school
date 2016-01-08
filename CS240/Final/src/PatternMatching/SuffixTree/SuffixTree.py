from ..Matching import Matching


class SuffixTree(Matching):
    def __init__(self, needle, haystack):
        Matching.__init__(self, needle, haystack)
        self.build_trie()

    def build_trie(self):
        pass

    def find(self):
        pass