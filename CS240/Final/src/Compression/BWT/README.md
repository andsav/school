Burrows-Wheeler Transform (BWT)
=========================

* Transforms source text to a coded text with the same letters in different order
* **The coded text will be more easily compressible with MTF**
* Decoding is more efficient than encoding
* Best so far

bzip2 uses BWT -> MTF -> RLE -> Huffman -> Profit

How?
------
* Given text T
* Compute all *rotations* of T (abab$, $abab, b$aba, ab$ab, bab$a)
* Sort lexicographically, and output to matrix:

$abab<br/>
ab$ab<br/>
abab$<br/>
b$aba<br/>
bab$a<br/>

* BWT is the last column of this matrix: bb$aa

Reversing
----------


Overview
----------
* Encoding cost: O(n^2) (using radix sort)
* Decoding cost: O(n)