Summary
=========

* Sorted array: array, search is O(logn), the rest is slow, trivial to implement, doesn't extend
* Self-balancing trees: trees (references), all operations O(logn), extends to external memory (B-tree)
* Hashing: arrays, operations O(1) on average, extends to external memory

Self-rearranging Arrays
==================

* Applies to: implementations of search on unordered arrays
* Idea: keys that are searched for frequently should be in the front

Optimal Static Ordering
-----------------------
If we know the frequencies f(k) of search for every key k, sort the array by decreasing frequency

Move-to-front Heuristic
------------------------
When search for k is successful, move key k to the front of the array

Transpose Heuristic
---------------------
When search for k is successful, move k one index forward

Ordered Arrays
=================

* Search is O(log n) with binary search
* The idea of binary search is to always look for k halfway between *left* and *right* and recursively pin down its position

Interpolation Search
------------------
Guide where you search by where you would expect the key to be.
For example, if you search(100) between 40 and 120, you would expect 100 to be 3/4 down the range

Galloping
-----------
* For huge arrays
* You can search while the tail is still loading
* Start at array[0] and make the jump interval bigger and bigger (e.g.: 1, 2, 4, 8,...)
* If hopped past the key we are looking for (key > k), stop and do binary search between current element and last

Skip Lists
=================

* Applies to: implementations of search with ordered lists
* Idea: speed up search by storing additional references

* We have a series of lists S_0, S_1... S_n
* S_0 stores the whole ordered list
* Other lists store "sentinels" (references to above/below)
* While doing search, start with S_n, and scan forward/downward until we are in S_0 (very similar to binary search)

* Expected runtime: O(log n)