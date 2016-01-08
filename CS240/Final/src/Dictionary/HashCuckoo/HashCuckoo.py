from ...config import *
from ..Dictionary import Dictionary
from ..Table import Table


class HashCuckoo(Dictionary):

    def __init__(self, array):
        self.table = Table(default_table_size)
        self.build(array)

    def h1(self, key):
        return key % self.table.__len__()

    def insert_at(self, kvp, page):
        pass

    def insert(self, kvp):
        pass

    def search_at(self, key, page):
        pass

    def search(self, key, root=None):
        pass

    def delete(self, key):
        pass