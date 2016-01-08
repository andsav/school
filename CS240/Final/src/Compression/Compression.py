from abc import ABCMeta, abstractmethod


class Compression(object):
    __metaclass__ = ABCMeta

    def __init__(self, text):
        self.text = text
        self.bitstring = ""

    @abstractmethod
    def encode(self):
        pass

    @abstractmethod
    def decode(self):
        pass
