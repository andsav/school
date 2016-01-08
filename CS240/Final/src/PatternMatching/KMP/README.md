Knuth–Morris–Pratt
===================

* m = length of needle, n = length of haystack
* Compute failure function - O(m)
* Parse in the resulting e-NFA - O(n)
* Runtime does not depend on length of alphabet: O(m + n)