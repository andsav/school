import MapReduce
import sys

mr = MapReduce.MapReduce()


def mapper(record):
    mr.emit_intermediate(record[0], 1)


def reducer(key, list_of_values):
    mr.emit((key, len(list_of_values)))


if __name__ == '__main__':
    input_file = open(sys.argv[1])
    mr.execute(input_file, mapper, reducer)
