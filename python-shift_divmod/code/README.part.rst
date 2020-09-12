PURPOSE
-------

This implement faster divmod() (and mod()) for moduli with trailing 0 bits.

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
    # Or use:
    # modder = ShiftDivMod.from_int(base << shift)

    # same as divmod(x, (base << shift))
    print( modder.divmod(x) )

NOTES
-----

This was proposed as a proof-of-concept for
a potential improvement for the built in cpython3
operations here: https://bugs.python.org/issue41487 but was
rejected.
