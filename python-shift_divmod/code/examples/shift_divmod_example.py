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
import sys

OPT = "builtinops"


def mytest(p):
    pint2p = p << p
    myrange = range(1000)
    if OPT == "builtinpow":
        ret = pow(2, (1 << p), pint2p)
    elif OPT == "gmpy":
        from gmpy2 import mpz
        ret = mpz(2)
        pint2p = mpz(pint2p)
        mod = mpz(10 ** 20)
        for i in myrange:
            print('sq', i, flush=True)
            ret *= ret
            print('mod', i, flush=True)
            ret %= pint2p
            print('after mod', i, (ret % mod), flush=True)
    elif OPT == "builtinops":
        ret = 2
        for i in myrange:
            print('sq', i, flush=True)
            ret *= ret
            print('mod', i, flush=True)
            ret %= pint2p
            print('after mod', i, (ret % 10 ** 20), flush=True)
    else:
        from shift_divmod import ShiftDivMod
        ret = 2
        if OPT == "test":
            for m in range(2, 10000):
                print("Testing m =", m, flush=True)
                modder = ShiftDivMod.from_int(m)
                for x in range(10*m):
                    d, mod = modder.divmod(x)
                    assert x == d*modder.n + mod
                    assert 0 <= mod < modder.n
                    assert (d, mod) == divmod(x, m)

        elif OPT == "shiftmodpre":
            m = ShiftDivMod(p, p)
            for i in myrange:
                print('sq', i, flush=True)
                ret *= ret
                print('mod', i, flush=True)
                # m = ShiftDivMod.from_int(pint2p)
                d, mod = m.divmod(ret)
                if 0:
                    assert ret == d*m.n + mod
                    assert 0 <= mod < m.n
                ret = mod
                # ret = m.mod(ret)
                print('after mod', i, (ret % 10 ** 20), flush=True)
        elif OPT == "gen_shift_mod":
            m = ShiftDivMod.from_int(pint2p)
            for i in myrange:
                print('sq', i, flush=True)
                ret *= ret
                print('mod', i, flush=True)
                ret = m.mod(ret)
                print('after mod', i, (ret % 10 ** 20), flush=True)
        elif OPT == "jitshift":
            for i in myrange:
                print('sq', i, flush=True)
                ret *= ret
                print('mod', i, flush=True)
                ret = ShiftDivMod.from_int(pint2p).mod(ret)
                print('after mod', i, (ret % 10 ** 20), flush=True)
    return ret % (p*p) // p


def main(which):
    global OPT
    OPT = which
    '''
    if which == "builtinpow":
        OPT = 1
    elif which == "builtinops":
        OPT = 0
    elif which == "shiftmod":
        OPT = 2
    else:
        raise Exception("unknown choice")
    '''
    x = mytest(9816593)
    print(x)
    return


if __name__ == "__main__":
    main(sys.argv[1])
