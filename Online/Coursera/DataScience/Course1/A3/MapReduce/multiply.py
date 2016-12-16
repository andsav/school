import MapReduce
import sys

mr = MapReduce.MapReduce()


def mapper(record):
    for i in range(5):
        key = record[0] == "a" and (record[1], i) or (i, record[2])
        mr.emit_intermediate(key, record)


def reducer(key, list_of_values):
    sum = 0
    for a in filter(lambda x: x[0] == "a", list_of_values):
        for b in filter(lambda x: x[0] == "b", list_of_values):
            if a[2] == b[1]:
                sum += a[3] * b[3]

    mr.emit(tuple(list(key) + [sum]))


if __name__ == '__main__':
    input_file = open(sys.argv[1])
    mr.execute(input_file, mapper, reducer)
