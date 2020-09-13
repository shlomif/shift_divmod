PURPOSE
-------

This distribution implements faster divmod() (and ``.mod()``) operations
for moduli with a large number of trailing 0 bits (where the div/mod base
is divisible by ``2 ** n`` for an integer `n`).

It should yield the same result as the built-n divmod() function for
positive numerators (its behaviour for negative ones is currently
untested and undefined).

INSTALLATION
------------

pip3 install shift_divmod

USAGE
-----

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
-----

The code from which this distribution has been derived, was proposed as a
proof-of-concept for a potential improvement for the built in cpython3
operations here: https://bugs.python.org/issue41487 . However, changing cpython3
in this manner was rejected.

libdivide ( https://github.com/ridiculousfish/libdivide ) provides a
different, but also interesting, approach for optimizing division.

