import pytest
import path

from src.PatternMatching.BoyerMoore import BoyerMoore

find = BoyerMoore.BoyerMoore("hel", "hello kitty")

assert(find.find() is True)