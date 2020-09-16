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

#ifdef USE_SHIFT
#include "shift_divmod_gmp.h"
#endif

#ifdef VERBOSE
#define v_printf(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define v_printf(fmt, ...)                                                     \
    {                                                                          \
    }
#endif

static unsigned long mytest(const unsigned long p)
{
#ifdef USE_SHIFT
    shift_divmod_gmp__type pint2p;
    mpz_t pmpz;
    mpz_init_set_ui(pmpz, p);
#if 0
    shift_divmod_gmp__init(&pint2p, pmpz, p);
#else
    mpz_mul_2exp(pmpz, pmpz, p);
    shift_divmod_gmp__init_from_num(&pint2p, pmpz);
    shift_divmod_gmp__scratch__type scratch;
    shift_divmod_gmp__scratch__init(&scratch, &pint2p);
#endif
    mpz_clear(pmpz);
#else
    mpz_t pint2p;
    mpz_init_set_ui(pint2p, p);
    mpz_mul_2exp(pint2p, pint2p, p);
// #define OPT
#ifdef OPT
    mpz_t t2int2p;
    mpz_init_set_ui(t2int2p, 1);
    mpz_mul_2exp(t2int2p, t2int2p, p);
#endif
#endif

    mpz_t ret, ret_div;
    mpz_init_set_ui(ret, 2);
    mpz_init(ret_div);

#ifdef OPT
    mpz_powm(ret, ret, t2int2p, pint2p);
#else
    // for (unsigned long i = 0; i < p; ++i)
    for (unsigned long i = 0; i < 1000; ++i)
    {
        v_printf("sq %lu\n", i);
        mpz_mul(ret, ret, ret);

#if 0
        gmp_fprintf(stdout, "%Zd\n", ret);
#endif

        v_printf("mod %lu\n", i);

#ifdef USE_SHIFT
        shift_divmod_gmp__divmod(&pint2p, &scratch, ret_div, ret, ret);
#else
#if 0
        if (mpz_cmp(ret, pint2p) >= 0)
#endif
        {
            mpz_mod(ret, ret, pint2p);
        }
#endif

        v_printf("after mod %lu\n", i);
    }
#endif
#if 0
    gmp_fprintf(stdout, "%Zd\n", ret);
#endif
    mpz_mod_ui(ret, ret, p * p);
    const unsigned long rett = mpz_get_ui(ret);
    mpz_clear(ret);
    mpz_clear(ret_div);
#ifdef USE_SHIFT
    shift_divmod_gmp__scratch__clear(&scratch);
    shift_divmod_gmp__clear(&pint2p);
#else
    mpz_clear(pint2p);
#endif
#ifdef OPT
    mpz_clear(t2int2p);
#endif

    return rett / p;
}

int main()
{
    const unsigned long x = mytest(9816593);
    printf("result = %lu\n", x);
    return 0;
}
