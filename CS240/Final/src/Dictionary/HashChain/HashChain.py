from ...config import *
from ..Dictionary import Dictionary
from ..Table import Table
from ..LinkedList import LinkedList


class HashChain(Dictionary):

    """
        Table of linked lists
    """

    def __init__(self, array):
        self.table = Table(default_table_size)
        self.build(array)

    def h1(self, key):
        return key % self.table.__len__()

    def insert_at(self, kvp, page):
        if page is None:
            page = LinkedList(kvp)
        else:
            self.insert_at(kvp, page.next)

    def insert(self, kvp):
        self.insert_at(kvp, self.table[self.h1(kvp[0])])

    def search_at(self, key, page):
        if page is None:
            return False, page
        elif page.cargo[0] == key:
            return True, page
        else:
            return self.search_at(key, page.next)

    def search(self, key, root=None):
        return self.search_at(self.h1(key))


    def delete(self, key):
        search = self.search(key)

        if not search[0]:
            return
        else:
            search[1] = search[1].next