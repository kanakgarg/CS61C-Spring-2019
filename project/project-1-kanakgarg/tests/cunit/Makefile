CUNIT = -L/home/ff/cs61c/cunit/install/lib -I/home/ff/cs61c/cunit/install/include -lcunit


build: test.h test.c ../../utils.c ../../utils.h ../../string-helpers.h ../../string-helpers.c
	gcc -std=c99 -g test.c ../../utils.c ../../string-helpers.c -o unit-tests $(CUNIT)

run:
	./unit-tests

run-valgrind:
	valgrind --leak-check=full ./unit-tests

clean:
	rm -rf unit-tests*

.PHONY: build clean run run-valgrind
