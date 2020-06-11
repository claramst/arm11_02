CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: clean

encode.o: encode.c encode.h

hashmap.o: hashmap.c hashmap.h

tokenizer.o: tokenizer.c tokenizer.h


clean:
	rm -f $(wildcard *.o)
