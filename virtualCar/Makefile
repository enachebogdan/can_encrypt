PROJECT_NAME=virtualcar
CC=gcc

SOURCES = src/include/lib.h src/include/lib.c

all:
	$(CC) -fPIC -pthread -c $(SOURCES)
	ar rcs $(PROJECT_NAME).a lib.o


clean:
	rm -f $(PROJECT_NAME) *.o *.a
