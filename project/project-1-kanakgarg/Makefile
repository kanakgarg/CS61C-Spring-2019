Flags=-Wall -g -std=c99


61ccc : main.o tokenizer.o tokenizer-errors.o tokens.o string-helpers.o utils.o preprocessor.o
	gcc $(Flags) -o 61ccc main.o tokenizer.o tokenizer-errors.o tokens.o string-helpers.o utils.o preprocessor.o

main.o : main.c tokens.h tokenizer.h preprocessor.h
	gcc $(Flags) -c main.c

tokens.o : tokens.c tokens.h
	gcc $(Flags) -c tokens.c

tokenizer.o : tokenizer.c string-helpers.h utils.h tokenizer.h tokens.h preprocessor.h tokenizer-errors.h
	gcc $(Flags) -c tokenizer.c

tokenizer-errors.o : tokenizer-errors.c tokenizer-errors.h utils.h tokens.h string-helpers.h
	gcc $(Flags) -c tokenizer-errors.c

string-helpers.o: string-helpers.c string-helpers.h
	gcc $(Flags) -c string-helpers.c

utils.o: utils.c utils.h
	gcc $(Flags) -c utils.c

preprocessor.o: preprocessor.c preprocessor.h string-helpers.h
	gcc $(Flags) -c preprocessor.c

clean:
	rm -f *.o
	rm -f 61ccc

.PHONY: clean
