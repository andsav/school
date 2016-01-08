import math


class Heap(list):
    def __init__(self, array):
        list.__init__(list())
        self.extend(array)
        self.heapify()

    def heapify(self):
        if self.__len__() % 2 == 0:
            self.append(None)

        for i in range(self.__len__() - 1, 0, -2):
            max_leaf = i if self[i] >= self[i-1] else i-1
            if self.parent(i) < self[max_leaf]:
                self.swap_parent(max_leaf)

        if self[-1] is None:
            self.pop()

    # Simulating tree
    def root(self):
        return self[0]

    def parent(self, k):
        return self[parent_index(k)]

    def left(self, k):
        return self[left_index(k)]

    def right(self, k):
        return self[right_index(k)]

    def swap(self, k1, k2):
        temp = self[k1]
        self[k1] = self[k2]
        self[k2] = temp

    def swap_parent(self, k):
        self.swap(k, parent_index(k))

    # Priority Queue operations

    def insert_key(self, key):
        self.append(key)
        j = self.__len__() - 1

        while j > 0 and self.parent(j) < self[j]:
            self.swap_parent(j)
            j = parent_index(j)

    def delete_max(self):
        ret = self.root()
        self[0] = self[self.__len__() - 1]
        self.pop()

        j = 0
        n = self.__len__() - 1

        while j <= n:
            max_index = j

            if left_index(j) <= n and self.left(j) > self[j]:
                max_index = left_index(j)

            if right_index(j) <= n and self.right(j) > self[max_index]:
                max_index = right_index(j)

            if max_index == j:
                break
            else:
                self.swap(j, max_index)
                j = max_index

        return ret

def parent_index(k):
    return int(math.floor((k - 1) / 2))


def left_index(k):
    return 2 * k + 1


def right_index(k):
    return 2 * k + 2