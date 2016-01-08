Run-Length Encoding
====================

* Variable-length code with a fixed decoding dictionary
* Dictionary not explicitly stored
* Not character-encoding (multiple characters represented by one dictionary entry)
* Source alphabet and coded alphabet are both {0, 1}

Run
----

* 0's and 1's in S may be repeated many times in a row (called a "run")
* Idea: sequence of integers indicating run lengths (alternating 0's and 1's sequences)

How to encode in binary?
----------------

(...)

Summary
----------
* No compression until run-length k >= 6
* Expansion when run-length k = 2 or 4
* Used in TIFF image format