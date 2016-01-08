from ..Matching import Matching


class KMP(Matching):
    def __init__(self, needle, haystack):
        Matching.__init__(self, needle, haystack)
        self.failure = [None] * self.m()
        self.compute_failure()

    """
        Compute the failure function (prefix table)
        For each i <= m, we compute the length of the longest prefix P[0...i-1]
            that is also a proper suffix of P[0...i-1]


        For P = "cocacola":

        P[1..i-1]   P[0..i]     failure[i]
        -           -            0
        o           c            0
        oc          co           1
        oca         coc          0
        ocac        coca         1
        ocaco       cocac        2
        ocacol      cocaco       0
        ocacola     cocacol      0

    """
    def compute_failure(self):
        self.failure[0] = 0
        i = 1
        j = 0
        while i < self.m():
            if self.needle[i] == self.needle[j]:
                self.failure[i] = j + 1
                i += 1
                j += 1
            elif j > 0:
                j = self.failure[j-1]
            else:
                self.failure[i] = 0
                i += 1

    """
        Compare the needle character by character with substrings of the haystack from left to right
        When mismatch, jump right by 1 + failure[i] where i is the index of the substring mismatched
    """
    def find(self):
        i = j = 0

        while i < self.n():

            if self.haystack[i] == self.needle[j]:
                if j == self.m() - 1:
                    return True
                else:
                    i += 1
                    j += 1
            elif j > 0:
                j = self.failure[j-1]
            else:
                i += 1

        return False