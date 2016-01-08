Suffix Tree
==============

* If we want to search for many needles P within the same fixed haystack T
* Idea: preprocess the haystack rather than the needle

Observation
------------
P is a substring of T iff P is a prefix of some suffix of T

* Suffix trie: trie that stores all suffixes of a text T
* Suffix tree: compressed suffix trie of T