Lempel-Ziv-Welch (LZW)
===========

Family of *adaptive* compression algorithms.
LZW and derivatives are used in zip, gzip, PNG, GIF, etc.

Adaptive dictionaries
-----------
* For ASCII, UTF-8 and RLE, dictionary is *fixed*
* For Huffman, dictionary is static: it is the same for the entire encoding/decoding
* Adaptive encoding:
    * There is an initial dictionary D[0] (fixed)
    * For i >= 0, D[i] is used to determine the i'th output character
    * After writing the i'th character to output, both encoder and decoder update D[i] to D[i+1]

Encoder and decoder have the same information and both operations have the same cost.

Patterns
---------
Huffman and RLE take advantage of frequent or repeated *single characters*
But certain *substrings* are much more frequent than others (for example, in English TH, ER, ON, etc.)


Main Idea
----------
Each character in the coded text C refers either to a single character in E or a substring of S that both encoder and decoder have already seen
We don't include the dictionary with the data, we build it as we encode/decode

Best explanation: <https://www.youtube.com/watch?v=j2HSd3HCpDs>