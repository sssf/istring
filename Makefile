all: test

# Tex settings
C_COMPILER   = gcc
C_OPTIONS    = -ggdb -Wall -std=c99

# Clean settings
GEN_EXTENSIONS = *.o *.out

# SCM
VC_PROGRAM = git

# Fix test output
TEST_PASS = 's/...passed/\x1b[1;32m   PASSED\x1b[m/g'
TEST_FAIL = 's/...FAILED/\x1b[1;31m   FAILED\x1b[m/g'

clean:
	rm -f $(GEN_EXTENSIONS) bin/unittests *.orig

commit: beautify clean test
	$(VC_PROGRAM) pull; $(VC_PROGRAM) commit; $(VC_PROGRAM) push

%:	%.c
	$(C_COMPILER) $(C_OPTIONS) $< -o $@

beautify:
	astyle -A7 source/*.c source/*.h tests/*.c tests/*.h

test: tests/unittests.c source/istring.c source/istring.h
	@mkdir -p bin
	@$(C_COMPILER) $(C_OPTIONS) tests/unittests.c source/istring.c -o bin/unittests -lcunit
	@./bin/unittests | sed $(TEST_PASS)  | sed $(TEST_FAIL)

