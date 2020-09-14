/*
 * shift_divmod_gmp.h
 * Copyright (C) 2020 Shlomi Fish <shlomif@cpan.org>
 *
 * Distributed under the terms of the MIT license.
 */

#ifndef SHIFT_DIVMOD_GMP_H
#define SHIFT_DIVMOD_GMP_H

#include <gmp.h>

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

static void shift_divmod_gmp__divmod(shift_divmod_gmp__type *const modder,
    mpz_t ret_div, mpz_t ret_mod, mpz_t inp)
{
    if (mpz_cmp(inp, modder->n) >= 0)
    {
        mpz_div_2exp(modder->shifted_input, inp, modder->shift);
        mpz_fdiv_qr(ret_div, modder->shifted_input, modder->shifted_input,
            modder->base);
        mpz_mul_2exp(
            modder->shifted_input, modder->shifted_input, modder->shift);
        mpz_and(ret_mod, inp, modder->mask);
        mpz_ior(ret_mod, modder->shifted_input, ret_mod);
    }
    else
    {
        mpz_set_ui(ret_div, 0);
        mpz_set(ret_mod, inp);
    }
}

#endif /* !SHIFT_DIVMOD_GMP_H */
