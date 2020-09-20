Flags=-Wall -g -std=c99
LinkingInfo=-L/home/ff/cs61c/bin/static -l61Ccc-notcgen

61Ccc : main.o cgen.o cgen-helpers.o instructions.o
	gcc $(Flags) -o 61Ccc main.o cgen.o cgen-helpers.o instructions.o $(LinkingInfo)

main.o : main.c tokens.h tokenizer.h parser.h parser-errors.h preprocessor.h ast.h ast-print.h dast.h decorate.h decorate-errors.h cgen.h cgen-helpers.h
	gcc $(Flags) -c main.c

cgen.o: cgen.c cgen.h dast.h instructions.h decorate.h cgen-helpers.h
	gcc $(Flags) -c cgen.c

cgen-helpers.o: cgen-helpers.h
	gcc $(Flags) -c cgen-helpers.c

instructions.o: instructions.c instructions.h
	gcc $(Flags) -c instructions.c
clean:
	rm -f *.o
	rm -f 61Ccc

.PHONY: clean
