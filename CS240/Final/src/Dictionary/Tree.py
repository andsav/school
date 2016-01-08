class Tree:

    def __init__(self, kvp=(None, None)):
        self.k = kvp[0]
        self.v = kvp[1]
        self.left = None
        self.right = None
        self.parent = None

    def is_leaf(self):
        return self.left is None and self.right is None

    def set_left(self, kvp):
        self.left = Tree(kvp)
        self.left.parent = self

    def set_right(self, kvp):
        self.right = Tree(kvp)
        self.right.parent = self

    def remove(self, node):
        if self.left == node:
            self.left = None
        else:
            self.right = None