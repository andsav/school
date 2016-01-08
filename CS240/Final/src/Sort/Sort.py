from abc import ABCMeta, abstractmethod

class Sort(object):
    __metaclass__ = ABCMeta

    def __init__(self, array):
        self.array = array

    @abstractmethod
    def sort(self):
        pass
