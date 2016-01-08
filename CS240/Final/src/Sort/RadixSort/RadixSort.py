from ..Sort import Sort


class RadixSort(Sort):
    def __init__(self, array):
        Sort.__init__(self, array)
        self.buckets = [None] * 10

    def sort(self):
        pass