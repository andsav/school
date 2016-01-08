Compression
===============

Alice -> (encoding/compression) -> bitstring -> (decoding/decompression) -> Bob

We want to optimize:

* Size
* Processing Speed
* Reliability
* Security

We concentrate on physical, lossless compression.

Algorithms
------------

* Huffman
* Run-length encoding
* LZW

Summary
-----------

|          Huffman         | Run-Length                               | LZW                                 |
|:------------------------:|------------------------------------------|-------------------------------------|
| Variable-length          | Variable-length                          | Fixed-length                        |
| Single-character         | Multi-character                          | Multi-character                     |
| 2-pass                   | 1-pass                                   | 1-pass                              |
| 60% English text         | Bad on text                              | 45% English text                    |
| Optimal 01-prefix-code   | Good if long runs (e.g. pictures)        | Good on English text                |
| Must send dictionary     | Can be worse than ASCII                  | Can be worse than ASCII             |
| Part of pkzip, JPEG, MP3 | Old picture formats (TIFF), fax machines | GIF, ZIP, PNG, some variants of PDF |


Text transformations
====================

Idea: preprocess the text first so that we get:

* frequently repeating characters and/or
* frequently repeating substrings

Algorithms
-------------
* Move-to-front
* BurrowsWheeler

In practice
===========

gzip
-------

T -> (lzw derivative) -> Output

bzip2
--------

T -> (bwt) -> (mtf) -> (rle) -> (Huffman) -> Output + Dictionary