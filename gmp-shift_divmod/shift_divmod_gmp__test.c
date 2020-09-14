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

#include "rinutils/rin_cmocka.h"
#include "rinutils/unused.h"
#include "rinutils/bit_rw.h"

static void main_tests(void **state GCC_UNUSED)
{
    for (size_t shift = 2; shift < 10; ++shift)
    {
        for (size_t base = 1; base < 10; base += 2)
        {
            size_t m = (base << shift);
            mpz_t mpbase;
            mpz_init_set_ui(mpbase, base);
            shift_divmod_gmp__type modder;
            shift_divmod_gmp__init(&modder, mpbase, shift);
            assert_int_equal(mpz_cmp_ui(modder.n, m), 0);
            /* code */
            for (size_t x = 0; x < m * 10; ++x)
            {
                mpz_t mod, mpx;
                mpz_init_set_ui(mod, 0);
                mpz_init_set_ui(mpx, x);
                shift_divmod_gmp__mod(&modder, mod, mpx);
                assert_int_equal(mpz_cmp_ui(mod, (x % m)), 0);
                mpz_clear(mod);
                mpz_clear(mpx);
            }
            shift_divmod_gmp__clear(&modder);
            mpz_clear(mpbase);
        }
    }
}

int main(void)
{
    // plan([% num_tests %]);
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(main_tests),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
