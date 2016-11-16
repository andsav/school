import MapReduce
import sys

mr = MapReduce.MapReduce()


def mapper(record):
    mr.emit_intermediate(record[1], record)


def reducer(key, list_of_values):
    order = []
    listings = []

    for item in list_of_values:
        if item[0] == "order":
            order = item
        else:
            listings.append(item)

    for item in listings:
        mr.emit(order + item)


if __name__ == '__main__':
    input_file = open(sys.argv[1])
    mr.execute(input_file, mapper, reducer)
