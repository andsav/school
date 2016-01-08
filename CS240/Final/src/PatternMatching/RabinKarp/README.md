Rabin-Karp
===========

Also called fingerprinting.

* For all substrings of length needle.len() in haystack, compute a hash-function
* Initially runtime would be O(mn) for search miss (since hashing requires m)
    * Idea: **fingerprints**
    * Use previous hash to compute next hash. O(1) except first one
    * O(m + n) expected runtime, O(mn) worst-case but very unlikely
* Extends to 2d patterns and other generalizations
