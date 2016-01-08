from ..Compression import Compression


class LZW(Compression):
    def __init__(self, text):
        Compression.__init__(self, text)
        self.dictionary = { }

    def encode(self):
        pass

    def decode(self):
        pass