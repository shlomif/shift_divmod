#
# Makefile
# Shlomi Fish, 2020-09-13 17:48
#

all: divmod-gcc.exe

divmod-gcc.exe: shift_divmod_gmp.c
	gcc -o $@ -O3 -flto -fwhole-program -fomit-frame-pointer -march=native -Wall -Wextra $< -lgmp

# vim:ft=make
#
