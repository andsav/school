from ..Matching import Matching


class BoyerMoore(Matching):
    def __init__(self, needle, haystack):
        Matching.__init__(self, needle, haystack)
        self.suffix = []
        self.last = {}
        self.compute_last_function()

    """
        Bad match heuristic : last index where the character is found
        For each character c, last[c] = length - lastIndex - 1
    """
    def compute_last_function(self):
        for (i, c) in enumerate(self.needle):
            self.last[c] = self.m() - i -1

    def bad_character(self, c):
        if c in self.last:
            return self.last[c]
        else:
            return -1

    '''
    # Good suffix heuristic
    def compute_suffix_function(self):
        pass
    '''

    def find(self):
        i = self.m() - 1

        while i < self.n():
            j = self.m() - 1
            while j >= 0 and self.haystack[i] == self.needle[j]:
                i -= 1
                j -= 1
            if j == -1:
                return True
            else:
                i = i + self.m() - 1 - self.bad_character(self.haystack[j])

        return False
