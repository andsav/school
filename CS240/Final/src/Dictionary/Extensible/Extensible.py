from ...config import *
from ..Dictionary import Dictionary
from ..Table import Table


class Page:
    def __init__(self, local_depth, array=[]):
        self.local_depth = local_depth
        self.array = array

class Extensible(Dictionary):

    def __init__(self, array):
        self.page_size = default_page_size
        self.global_depth = 2
        self.build(array)

    def double(self):
        pass

    def insert(self, kvp):
        pass

    def search(self, key, root=None):
        pass

    def delete(self, key):
        pass