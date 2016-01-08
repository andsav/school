Accessing data in external memory is expensive. We want to minimize external memory access.
Assume memory is stored in blocks called pages of size S.

Extensible Hashing
=================

* Similar to a B-Tree of size 1 and max size S at the leaves.
* Memory is divided into *blocks* of size S (contain at most S items)
* We have a table in internal memory of size 2^d, where *d* is the _order_ (or _global depth_)
* Each cell in the table points at a block in external memory. Many cells can point to the same block (*we are combining pages to save space*)
* Each block B stores k_B <= d called the _local depth_. All the keys pointing to the same block have the same k_B leading digits
    * e.g.) if h(k) starts with 010, it is stored at D[010]
* Hash values are length L bitstrings
* We chose d <= L big enough so that the pages don't get full, and smallest possible

![Illustration] (http://i.imgur.com/HgTSboT.png)

Block Split and Directory Grow
----------------------------------

To Insert(k, v):

* Compute h(k), look up block B at D[first d digits of h(k)]
* If B has space left (#kvp at B < S), insert (k, v) into B
* Elseif B is full
    * **BLOCK SPLIT**: If local depth K_b < d, split B into 2 pages (say B_0 and B_1).
        * B_0 gets all the keys in B where the (K_b+1)th bit is 0
        * B_1 gets all the keys in B where the (K_b+1)th bit is 1
        * B_0 and B_1 will have local depth K_b+1
        * Update 2^(d-K_b) references in D to point to B_0 and B_1
        * Now try insert again
    * **DIRECTORY GROW**: If K_b = d:
        * d++ (double the directory)
        * Copy link at D[010] to both D[0100] and D[0101]
        * We don't need to load these blocks or change their local depths!
        * Now try insert again