**Conventions**:

P[] = needle (pattern)
T[] = haystack (text)
m = P.length()
n = T.length()
Σ = alphabet

Summary
---------

|             | Bruteforce | KMP  | Boyer-Moore         | Rabin-Karp        | Suffix Tree       |
|-------------|------------|------|---------------------|-------------------|-------------------|
| Preprocess  | -          | O(m) | O(m + Σ.length)     | O(m)              | O(n^2) often O(n) |
| Search      | O(mn)      | O(n) | O(n) (often better) | expected O(m + n) | O(m)              |
| Extra space | -          | O(m) | O(m + Σ.length)     | O(1)              | O(n)              |