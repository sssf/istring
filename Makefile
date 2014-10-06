#tmp: run
all: test

# Tex settings
C_COMPILER   = gcc
C_OPTIONS    = -ggdb -Wall -std=c99

# Clean settings
GEN_EXTENSIONS = *.o *.out

# SCM
VC_PROGRAM = git

clean:
	rm -f $(GEN_EXTENSIONS) bin/unittests *.orig

commit: beautify clean test
	$(VC_PROGRAM) pull; $(VC_PROGRAM) commit; $(VC_PROGRAM) push

%:	%.c
	$(C_COMPILER) $(C_OPTIONS) $< -o $@

beautify:
	astyle -A7 source/*.c source/*.h tests/*.c tests/*.h

test: tests/unittests.c source/istring.c source/istring.h
	$(C_COMPILER) $(C_OPTIONS) tests/unittests.c source/istring.c -o bin/unittests -lcunit
	./bin/unittests


#run: main.c istring.c istring.h
#	$(C_COMPILER) $(C_OPTIONS) main.c istring.c -o main
#	./main
