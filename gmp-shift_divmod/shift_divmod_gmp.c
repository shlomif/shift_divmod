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
typedef struct
{
    mpz_t base, n, mask;
    unsigned long shift;
} shift_divmod_gmp__type;
#endif

static unsigned long mytest(const unsigned long p)
{
#ifdef USE_SHIFT
    shift_divmod_gmp__type pint2p;
    pint2p.shift = p;
    mpz_init_set_ui(pint2p.base, p);
    mpz_init(pint2p.n);
    mpz_mul_2exp(pint2p.n, pint2p.base, p);
    mpz_init_set_ui(pint2p.mask, 1);
    mpz_mul_2exp(pint2p.mask, pint2p.mask, pint2p.shift);
    mpz_sub_ui(pint2p.mask, pint2p.mask, 1);
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
    mpz_t ret;
    mpz_init_set_ui(ret, 2);
#ifdef OPT
    mpz_powm(ret, ret, t2int2p, pint2p);
#else
    // for (unsigned long i = 0; i < p; ++i)
    for (unsigned long i = 0; i < 1000; ++i)
    {
#ifdef USE_SHIFT
#ifdef VERBOSE
        printf("sq %lu\n", i);
#endif
        mpz_mul(ret, ret, ret);
#if 0
    gmp_fprintf(stdout, "%Zd\n", ret);
#endif
#ifdef VERBOSE
        printf("mod %lu\n", i);
#endif

        if (mpz_cmp(ret, pint2p.n) >= 0)
        {
            mpz_t foo, bar;
            mpz_init(foo);
            mpz_init(bar);
            mpz_div_2exp(foo, ret, pint2p.shift);
            mpz_mod(foo, foo, pint2p.base);
            mpz_mul_2exp(foo, foo, pint2p.shift);
            mpz_and(bar, ret, pint2p.mask);
            mpz_ior(ret, foo, bar);
            mpz_clear(foo);
            mpz_clear(bar);
        }
#ifdef VERBOSE
        printf("after mod %lu\n", i);
#endif
#else
        printf("sq %lu\n", i);
        mpz_mul(ret, ret, ret);
        printf("mod %lu\n", i);
        mpz_mod(ret, ret, pint2p);
        printf("after mod %lu\n", i);
#endif
    }
#endif
#if 0
    gmp_fprintf(stdout, "%Zd\n", ret);
#endif
    mpz_mod_ui(ret, ret, p * p);
    const unsigned long rett = mpz_get_ui(ret);
    mpz_clear(ret);
#ifdef USE_SHIFT
    mpz_clear(pint2p.base);
    mpz_clear(pint2p.n);
    mpz_clear(pint2p.mask);
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
