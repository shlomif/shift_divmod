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
HEADERS = shift_divmod_gmp.h
C_FILE := shift_divmod_gmp.c

divmod-gcc-with-shift.exe: $(C_FILE) $(HEADERS)
	gcc -D USE_SHIFT=1 -o $@ $(GCC_OPT_FLAGS) $< -lgmp

divmod-gcc-wo-shift.exe: $(C_FILE) $(HEADERS)
	gcc -o $@ $(GCC_OPT_FLAGS) $< -lgmp

divmod-clang-with-shift.exe: $(C_FILE) $(HEADERS)
	clang -D USE_SHIFT=1 -o $@ $(GCC_OPT_FLAGS) $< -lgmp

divmod-clang-wo-shift.exe: $(C_FILE) $(HEADERS)
	clang -o $@ $(GCC_OPT_FLAGS) $< -lgmp

# vim:ft=make
#
