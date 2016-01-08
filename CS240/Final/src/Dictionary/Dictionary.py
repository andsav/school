from abc import ABCMeta, abstractmethod

class Dictionary(object):
    __metaclass__ = ABCMeta

    """
        List of pairs (key-value) [(2,3), (4,5)]
    """

    def build(self, array):
        for item in array:
            self.insert(item)

    @abstractmethod
    def insert(self, kvp):
        pass

    @abstractmethod
    def search(self, key):
        pass

    @abstractmethod
    def delete(self, key):
        pass

