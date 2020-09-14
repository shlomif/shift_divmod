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

The code utilises the fact that `<https://en.wikipedia.org/wiki/Bitwise_operation> bitwise operations`
are fast, and the magic happens in this code:

::

    def divmod(self, inp):
        """calculate divmod(inp, self.n)"""
        if inp < self.n:
            return 0, inp
        div, mod = divmod((inp >> self.shift), self.base)
        return div, ((mod << self.shift) | (inp & self.mask))

(Or the equivalent but more bureaucratic C and gmplib code.)

Note that ``self.mask`` is precalculated to be
``((1 << self.shift) - 1)``.

