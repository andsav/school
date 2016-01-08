from ..Sort import Sort


class QuickSort(Sort):
    def __init__(self, array):
        Sort.__init__(self, array)
        self.pick_pivot()

    def pick_pivot(self):
        # randomize to avoid worst-case O(n^2)
        pass

    def do_sort(self, array):
        less = []
        equal = []
        greater = []

        if len(array) > 1:
            pivot = self.array[len(self.array)-1]
            for x in array:
                if x < pivot:
                    less.append(x)
                elif x == pivot:
                    equal.append(x)
                else:
                    greater.append(x)
            return self.do_sort(less) + equal + self.do_sort(greater)
        else:
            return array

    def sort(self):
        self.array = self.do_sort(self.array)
        return self.array