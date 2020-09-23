/*
 * # The Expat License
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
*/

#include <gmp.h>
#include <stdio.h>

#ifdef VERBOSE
#define v_printf(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define v_printf(fmt, ...)                                                     \
    {                                                                          \
    }
#endif

static unsigned long mytest(const unsigned long p)
{
    mpz_t pint2p;
    mpz_init_set_ui(pint2p, p);
    mpz_mul_2exp(pint2p, pint2p, p);

    mpz_t ret;
    mpz_init_set_ui(ret, 2);

    // for (unsigned long i = 0; i < p; ++i)
    for (unsigned long i = 0; i < 1000; ++i)
    {
        v_printf("sq %lu\n", i);
        mpz_mul(ret, ret, ret);

        v_printf("mod %lu\n", i);

        mpz_mod(ret, ret, pint2p);

        v_printf("after mod %lu\n", i);
    }
    mpz_mod_ui(ret, ret, p * p);
    const unsigned long rett = mpz_get_ui(ret);
    mpz_clear(ret);
    mpz_clear(pint2p);
    return rett / p;
}

int main()
{
    const unsigned long x = mytest(9816593);
    printf("result = %lu\n", x);
    return 0;
}
