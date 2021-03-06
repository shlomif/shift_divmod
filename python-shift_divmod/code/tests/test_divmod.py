#!/usr/bin/env python3

# The Expat License
#
# Copyright (c) 2020, Shlomi Fish
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import pytest  # noqa: F401

from shift_divmod import ShiftDivMod


def test_divmod():
    for m in range(2, 100):
        modder = ShiftDivMod.from_int(m)
        for x in range(10*m):
            d, mod = modder.divmod(x)
            assert modder.div(x) == d
            assert modder.mod(x) == mod
            assert x == d*modder.n + mod
            assert 0 <= mod < modder.n
            assert (d, mod) == divmod(x, m)

    for shift in range(2, 10):
        for base in range(1, 10, 2):
            modder = ShiftDivMod(base, shift)
            m = base << shift
            assert modder.n == m
            for x in range(10*m):
                d, mod = modder.divmod(x)
                assert x == d*modder.n + mod
                assert 0 <= mod < modder.n
                assert (d, mod) == divmod(x, m)
