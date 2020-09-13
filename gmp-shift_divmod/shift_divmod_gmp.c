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
    mpz_t base, n, mask, shifted_input, lower_and_masked_input;
    unsigned long shift;
} shift_divmod_gmp__type;

static void shift_divmod_gmp__init(
    shift_divmod_gmp__type *const dest, mpz_t base, const unsigned long shift)
{
    dest->shift = shift;
    mpz_init_set(dest->base, base);
    mpz_init(dest->n);
    mpz_mul_2exp(dest->n, dest->base, dest->shift);
    mpz_init_set_ui(dest->mask, 1);
    mpz_mul_2exp(dest->mask, dest->mask, dest->shift);
    mpz_sub_ui(dest->mask, dest->mask, 1);
    mpz_init(dest->shifted_input);
    mpz_init(dest->lower_and_masked_input);
}
static void shift_divmod_gmp__clear(shift_divmod_gmp__type *const modder)
{
    mpz_clear(modder->base);
    mpz_clear(modder->n);
    mpz_clear(modder->mask);
    mpz_clear(modder->shifted_input);
    mpz_clear(modder->lower_and_masked_input);
}

static void shift_divmod_gmp__mod(
    shift_divmod_gmp__type *const modder, mpz_t ret)
{
    if (mpz_cmp(ret, modder->n) >= 0)
    {
        mpz_div_2exp(modder->shifted_input, ret, modder->shift);
        mpz_mod(modder->shifted_input, modder->shifted_input, modder->base);
        mpz_mul_2exp(
            modder->shifted_input, modder->shifted_input, modder->shift);
        mpz_and(modder->lower_and_masked_input, ret, modder->mask);
        mpz_ior(ret, modder->shifted_input, modder->lower_and_masked_input);
    }
}
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
    shift_divmod_gmp__init(&pint2p, pmpz, p);
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

    mpz_t ret;
    mpz_init_set_ui(ret, 2);

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
        shift_divmod_gmp__mod(&pint2p, ret);
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
#ifdef USE_SHIFT
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
