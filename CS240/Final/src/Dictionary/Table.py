class Table(list):

    def __init__(self, length):
        list.__init__(list())
        for i in range(length):
            self.append(None)