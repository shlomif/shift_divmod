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

class ShiftDivMod:
    """Implement faster divmod() (and mod()) for moduli with trailing 0 bits"""
    def __init__(self, base, shift):
        """Initialize with n = (base << shift)"""
        self.base = base
        self.shift = shift
        self.mask = (1 << shift) - 1
        self.n = base << shift

    @classmethod
    def from_int(cls, n):
        """Initialize with base n"""
        s = 0
        for offset in (200000, 20000, 2000, 200, 20, 1):
            bits_mask = (1 << offset) - 1
            while n & bits_mask == 0:
                s += offset
                n >>= offset
        return cls(n, s)

    def divmod(self, inp):
        """calculate divmod(inp, self.n)"""
        if inp < self.n:
            return 0, inp
        div, mod = divmod((inp >> self.shift), self.base)
        return div, ((mod << self.shift) | (inp & self.mask))

    def mod(self, inp):
        """calculate inp % self.n . Uses .divmod() ."""
        return self.divmod(inp)[1]

    def div(self, inp):
        """calculate inp // self.n . Uses .divmod() ."""
        return self.divmod(inp)[0]
