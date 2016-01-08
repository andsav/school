from abc import ABCMeta, abstractmethod


class Matching(object):
    __metaclass__ = ABCMeta

    def __init__(self, needle, haystack):
        self.needle, self.haystack = needle, haystack

    def m(self):
        return len(self.needle)

    def n(self):
        return len(self.haystack)

    @abstractmethod
    def find(self):
        pass
