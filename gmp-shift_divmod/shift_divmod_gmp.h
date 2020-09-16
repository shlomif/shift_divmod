/*
 * shift_divmod_gmp.h
 * Copyright (C) 2020 Shlomi Fish <shlomif@cpan.org>
 *
 * Distributed under the terms of the MIT license.
 */

#ifndef SHIFT_DIVMOD_GMP_H
#define SHIFT_DIVMOD_GMP_H

#include <gmp.h>
#include "rinutils/unused.h"

typedef struct
{
    mpz_t base, n, mask;
    unsigned long shift;
} shift_divmod_gmp__type;

typedef struct
{
    mpz_t shifted_input;
} shift_divmod_gmp__scratch__type;

static void shift_divmod_gmp__init(shift_divmod_gmp__type *const dest,
    const mpz_t base, const unsigned long shift)
{
    dest->shift = shift;
    mpz_init_set(dest->base, base);
    mpz_init(dest->n);
    mpz_mul_2exp(dest->n, dest->base, dest->shift);
    mpz_init_set_ui(dest->mask, 1);
    mpz_mul_2exp(dest->mask, dest->mask, dest->shift);
    mpz_sub_ui(dest->mask, dest->mask, 1);
}

static void shift_divmod_gmp__scratch__init(
    shift_divmod_gmp__scratch__type *const dest,
    const shift_divmod_gmp__type *const src GCC_UNUSED)
{
    mpz_init(dest->shifted_input);
}
static const unsigned long SHIFT_DIVMOD_GMP__BIT_SHIFTS_TO_TRY[9] = {
    200000, 20000, 2000, 200, 20, 1, 0};

static void shift_divmod_gmp__init_from_num(
    shift_divmod_gmp__type *const dest, const mpz_t inp)
{
    mpz_t n;
    unsigned long shift = 0;
    mpz_init_set(n, inp);
    for (int i = 0; SHIFT_DIVMOD_GMP__BIT_SHIFTS_TO_TRY[i]; ++i)
    {
        const unsigned long try_shift = SHIFT_DIVMOD_GMP__BIT_SHIFTS_TO_TRY[i];
        mpz_t mask;
        mpz_init_set_ui(mask, 1);
        mpz_mul_2exp(mask, mask, try_shift);
        mpz_sub_ui(mask, mask, 1);
        mpz_t masked;
        mpz_init(masked);
        mpz_and(masked, mask, n);
        while (mpz_cmp_ui(masked, 0) == 0)
        {
            shift += try_shift;
            mpz_div_2exp(n, n, try_shift);
            mpz_and(masked, mask, n);
        }
        mpz_clear(mask);
        mpz_clear(masked);
    }
    shift_divmod_gmp__init(dest, n, shift);
    mpz_clear(n);
}

static void shift_divmod_gmp__clear(shift_divmod_gmp__type *const modder)
{
    mpz_clear(modder->base);
    mpz_clear(modder->n);
    mpz_clear(modder->mask);
}

static void shift_divmod_gmp__scratch__clear(
    shift_divmod_gmp__scratch__type *const scratch)
{
    mpz_clear(scratch->shifted_input);
}

static inline void shift_divmod_gmp__divmod(
    const shift_divmod_gmp__type *const modder,
    shift_divmod_gmp__scratch__type *const scratch, mpz_t ret_div,
    mpz_t ret_mod, const mpz_t inp)
{
    if (mpz_cmp(inp, modder->n) >= 0)
    {
        mpz_div_2exp(scratch->shifted_input, inp, modder->shift);
        mpz_fdiv_qr(ret_div, scratch->shifted_input, scratch->shifted_input,
            modder->base);
        mpz_mul_2exp(
            scratch->shifted_input, scratch->shifted_input, modder->shift);
        mpz_and(ret_mod, inp, modder->mask);
        mpz_ior(ret_mod, scratch->shifted_input, ret_mod);
    }
    else
    {
        if (mpz_cmp_ui(inp, 0) < 0)
        {
            mpz_fdiv_qr(ret_div, ret_mod, inp, modder->n);
        }
        else
        {
            mpz_set_ui(ret_div, 0);
            mpz_set(ret_mod, inp);
        }
    }
}

#endif /* !SHIFT_DIVMOD_GMP_H */
