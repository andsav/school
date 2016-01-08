import pytest
import path

from src.PatternMatching.KMP import KMP

find = KMP.KMP("kit", "hello kitty")

assert(find.find() is True)