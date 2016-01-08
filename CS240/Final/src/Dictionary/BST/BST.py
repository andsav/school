from ..Dictionary import Dictionary
from ..Tree import Tree


class BST(Dictionary):

    def __init__(self, array):
        self.tree = Tree()
        self.build(array)

    def build(self, array):
        for item in array:
            self.insert(item)

    def insert(self, kvp):
        search = self.search(kvp[0])[1]

        if search.k is None:
            search.k = kvp[0]
            search.v = kvp[1]
        elif search.k >= kvp[0]:
            search.set_left(kvp)
        else:
            search.set_right(kvp)

    def search(self, key, root=None):
        if root is None:
            root = self.tree

        if root.k == key:
            return True, root
        elif root.k < key:
            if root.left is not None:
                self.search(key, root.left)
        else:
            if root.right is not None:
                self.search(key, root.right)

        return False, root

    def delete(self, key):
        search = self.search(key)

        # Trivial
        if not search[0]:
            return

        node = search[1]

        # Node is leaf
        if node.is_leaf():
            node.parent.remove(node)

        # Node has 1 child
        elif node.left is None:
            node = node.right
        elif node.right is None:
            node = node.left

        # Node has 2 children
        else:
            node.k = node.right.left.k
            node.v = node.right.left.v
            self.delete(node.right.left.k)