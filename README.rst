==============================================================================
ShiftDivMod.  Implement faster divmod() for moduli with trailing 0 bits
==============================================================================
:Info: This is the README file for ShiftDivMod.
:Author: Shlomi Fish <shlomif@cpan.org>
:Copyright: © 2020, Shlomi Fish.
:Date: 2020-09-20
:Version: 0.4.3

.. index: README
.. image:: https://travis-ci.org/shlomif/shift_divmod.svg?branch=master
   :target: https://travis-ci.org/shlomif/shift_divmod

PURPOSE
=======

This distribution implements faster divmod() (and ``.mod()``) operations
for moduli with a large number of trailing 0 bits (where the div/mod base
is divisible by ``2 ** n`` for an integer `n`).

It should yield the same result as the built-n divmod() function for
positive numerators (its behaviour for negative ones is currently
untested and undefined).

Also provided is a port to C and gmplib (= GNU multiple precision):
https://github.com/shlomif/shift_divmod/tree/master/gmp-shift_divmod

INSTALLATION
============

pip3 install shift_divmod

USAGE
=====

::

    from shift_divmod import ShiftDivMod

    base = 997
    shift = 1200
    modder = ShiftDivMod(base, shift)
    # Alternative constructor which may require more
    # work and eventualy calls the default constructor
    modder = ShiftDivMod.from_int(base << shift)

    x = 10 ** 500
    # Same as divmod(x, (base << shift))
    print( modder.divmod(x) )

NOTES
=====

The code from which this distribution has been derived, was proposed as a
proof-of-concept for a potential improvement for the built in cpython3
operations here: https://bugs.python.org/issue41487 . However, changing cpython3
in this manner was rejected.

libdivide ( https://github.com/ridiculousfish/libdivide ) provides a
different, but also interesting, approach for optimizing division.

BENCHMARKS:
===========

On my system, I got these results after running
``python3 code/examples/shift_divmod_example.py bench`` (reformated
for clarity):

::

    {'val': 5206685, 'time': 38.86349368095398, 'reached': 1000,
     'interrupted': False, 'mode': 'gen_shift_mod'}
    {'val': 5206685, 'time': 39.018390417099, 'reached': 1000,
     'interrupted': False, 'mode': 'shiftmodpre'}
    {'val': mpz(5206685), 'time': 167.4433994293213, 'reached': 1000,
     'interrupted': False, 'mode': 'gmpy'}
    {'val': 3346424, 'time': 229.94409656524658, 'reached': 25,
     'interrupted': True, 'mode': 'builtinops'}

    System:    Kernel: 5.8.8-200.fc32.x86_64 x86_64 bits: 64
        Desktop: KDE Plasma 5.18.5
               Distro: Fedora release 32 (Thirty Two)
    CPU:       Info: Quad Core model: Intel Core i5-8259U
        bits: 64 type: MT MCP L2 cache: 6144 KiB
               Speed: 1600 MHz min/max: 400/3800 MHz Core speeds (MHz):
                    1: 1600 2: 1600 3: 1601
               4: 1600 5: 1600 6: 1601 7: 1601 8: 1601
    Graphics:  Device-1: Intel Iris Plus Graphics 655 driver: i915 v: kernel
               Display: server: Fedora Project
               X.org 1.20.8 driver: modesetting unloaded: fbdev,vesa
               resolution: 1920x1080~60Hz
               OpenGL: renderer: Mesa Intel Iris Plus
               Graphics 655 (CFL GT3) v: 4.6 Mesa 20.1.7

As can be noticed the shift_divmod based versions are over 4 times faster than
GMP and much faster than the builtinops which only completed 25 out of 1,000 iterations
before being interrupted. Note that for that use case, using GMP's modular exponentiation
seems even faster.

With the C and gmplib version:

- ``mpz_mod`` runs the benchmark in about 160 seconds.
- ``shift_divmod`` runs the benchmark in about 35 seconds.
- ``pypy3`` runs the pure-Python code in 25 seconds (!).

The Secret Sauce:
-----------------

The code utilises the fact that `bitwise operations <https://en.wikipedia.org/wiki/Bitwise_operation>`_
are fast, and the magic happens in this code (with some comments added for clarity):

::

    # Precalculating the object's field so that:
    # self.shift : a non-negative integer signifying the bit shift
    # self.base  : a non-negative integer which is shifted to
    # form the modulu
    # self.n = self.base << self.shift
    # self.mask = ((1 << self.shift) - 1)
    def divmod(self, inp):
        """calculate divmod(inp, self.n)"""
        if inp < self.n:
            return 0, inp
        div, mod = divmod((inp >> self.shift), self.base)
        return div, ((mod << self.shift) | (inp & self.mask))

(Or the equivalent but more bureaucratic C and gmplib code.)

COPYRIGHT
---------
Copyright © 2020, Shlomi Fish.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions, and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions, and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. Neither the name of the author of this software nor the names of
   contributors to this software may be used to endorse or promote
   products derived from this software without specific prior written
   consent.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
