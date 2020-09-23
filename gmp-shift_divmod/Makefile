#
# Makefile
# Shlomi Fish, 2020-09-13 17:48
#

all: \
	divmod-gcc-with-shift.exe \
	divmod-gcc-wo-shift.exe \
	divmod-clang-with-shift.exe \
	divmod-clang-wo-shift.exe \
	pure-gmp-bench.exe

GCC_OPT_FLAGS := -O3 -flto -fwhole-program -fomit-frame-pointer -march=native -Wall -Wextra
CLANG_OPT_FLAGS := -O3 -flto -fomit-frame-pointer -march=native -Wall -Wextra -Weverything -Wno-extra-semi-stmt
HEADERS = shift_divmod_gmp.h
C_FILE := shift_divmod_gmp.c

LIBS := -lgmp -ltcmalloc

divmod-gcc-with-shift.exe: $(C_FILE) $(HEADERS)
	gcc -D USE_SHIFT=1 -o $@ $(GCC_OPT_FLAGS) $< $(LIBS)

divmod-gcc-wo-shift.exe: $(C_FILE) $(HEADERS)
	gcc -o $@ $(GCC_OPT_FLAGS) $< $(LIBS)

divmod-clang-with-shift.exe: $(C_FILE) $(HEADERS)
	clang -D USE_SHIFT=1 -o $@ $(CLANG_OPT_FLAGS) $< $(LIBS)

divmod-clang-wo-shift.exe: $(C_FILE) $(HEADERS)
	clang -o $@ $(CLANG_OPT_FLAGS) $< $(LIBS)

pure-gmp-bench.exe: pure_gmp_benchmark.c
	gcc -o $@ $(GCC_OPT_FLAGS) $< $(LIBS)

test: divmod-test.exe
	CMOCKA_MESSAGE_OUTPUT=TAP prove ./divmod-test.exe

divmod-test.exe: shift_divmod_gmp__test.c $(HEADERS)
	gcc -D USE_SHIFT=1 -o $@ $(GCC_OPT_FLAGS) $< $(LIBS) -lcmocka

benchmark: all
	time ./divmod-gcc-with-shift.exe
	time ./divmod-gcc-wo-shift.exe

clean:
	rm -f *.exe

# vim:ft=make
#
