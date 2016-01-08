from abc import ABCMeta, abstractmethod


class Multi(object):
    __metaclass__ = ABCMeta

    """
        List of tuples [(x_0, x_1,... x_d), (x_0, x_1,... x_d)]
    """

    def build(self, array):
        for item in array:
            self.insert(item)

    @abstractmethod
    def insert(self, tup):
        pass

    @abstractmethod
    def range_search(self, tup1, tup2):
        pass