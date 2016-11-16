import MapReduce
import sys

mr = MapReduce.MapReduce()


def mapper(record):
    for word in record[1].split():
        mr.emit_intermediate(word, record[0])


def reducer(key, list_of_values):
    mr.emit((key, list(set(list_of_values))))


if __name__ == '__main__':
    input_file = open(sys.argv[1])
    mr.execute(input_file, mapper, reducer)
