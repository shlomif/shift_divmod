#
# Makefile
# Shlomi Fish, 2020-09-13 17:48
#

all: \
	divmod-gcc-with-shift.exe \
	divmod-gcc-wo-shift.exe \
	divmod-clang-with-shift.exe \
	divmod-clang-wo-shift.exe

GCC_OPT_FLAGS := -O3 -flto -fwhole-program -fomit-frame-pointer -march=native -Wall -Wextra

divmod-gcc-with-shift.exe: shift_divmod_gmp.c
	gcc -D USE_SHIFT=1 -o $@ $(GCC_OPT_FLAGS) $< -lgmp

divmod-gcc-wo-shift.exe: shift_divmod_gmp.c
	gcc -o $@ $(GCC_OPT_FLAGS) $< -lgmp

divmod-clang-with-shift.exe: shift_divmod_gmp.c
	clang -D USE_SHIFT=1 -o $@ $(GCC_OPT_FLAGS) $< -lgmp

divmod-clang-wo-shift.exe: shift_divmod_gmp.c
	clang -o $@ $(GCC_OPT_FLAGS) $< -lgmp

# vim:ft=make
#
