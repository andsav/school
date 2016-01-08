We want to implement d-dimensional dictionaries with range search

* We could reduce to 1-dimensional (combine into a single key), but then range search one one aspect becomes difficult
* We could use a dictionary for every aspect, but this is inefficient and wastes space

2 dimensions
-------------
We use partitions trees: kd-tree or quadtree

More dimensions
----------------
We use range trees