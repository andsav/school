Move-to-front
===============

* MTF is an adaptive compression algorithm, based on a similar idea than the heuristic for self-organizing search.
* Take advantage of the locality of data: if we see a character now, we'll probably see it again soon
* Source alphabet E, with size |E| = m, coded alphabet {0, 1, ..., m-1}

* Does not provide any compression on its own. Either combine with:
    * Prefix-free integer encoding (like in RLE)
    * Huffman coding

