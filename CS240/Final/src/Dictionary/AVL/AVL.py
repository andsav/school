from ..Dictionary import Dictionary
from ..Tree import Tree


class AVL(Dictionary):

    def __init__(self, array):
        self.tree = Tree()
        self.build(array)

    def build(self, array):
        for item in array:
            self.insert(item)

    def rotate_right(self, root=None):
        pass

    def rotate_left(self, root=None):
        pass

    def insert(self, kvp):
        pass

    def search(self, key, root=None):
        pass

    def delete(self, key):
        pass