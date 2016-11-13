import MapReduce
import sys

mr = MapReduce.MapReduce()


def mapper(record):
    mr.emit_intermediate("".join(sorted(list(record))), record)


def reducer(key, list_of_values):
    if len(list_of_values) != 2:
        mr.emit(tuple(list_of_values[0]))
        mr.emit(tuple(reversed(list_of_values[0])))


if __name__ == '__main__':
    input_file = open(sys.argv[1])
    mr.execute(input_file, mapper, reducer)