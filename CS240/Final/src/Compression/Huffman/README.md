Huffman Coding
===============

* Source alphabet is arbitrary (say E), coded alphabet is {0, 1}
* We build a binary trie to store the decoding dictionary D
* Each character in E is a leaf on the trie

Building the best trie
------------------------

* Determine the frequency of each character c in E
* Make |E| height-0 tries holding each character c in E
* Assign a "weight" to each trie: sum of frequencies of all letters
* Merge two tries with the least weights, new weight is their sum (ie add one bit to the encoding of each character)
* Repeat above until there is only 1 trie left; This is D

Data structure: min-ordered heap

Summary
---------

* Encoding is lots of work
* Constructed trie not necessarily unique
* Decoding trie must be transmitted along with the coded text
* Decoding is faster; this is an asymmetric scheme
* The best we can do for encoding one character at a time
* Need two passes through text: compute frequencies and actual encoding