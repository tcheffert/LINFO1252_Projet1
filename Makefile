CC = gcc
CFLAGS = -Wall -Werror
LIBS = -lpthread

FILES = philosophers philosophers_TAS philosophers_TATAS \
        producers_consumers prod_conso_TAS prod_conso_TATAS \
        readers_writers readers_writers_TAS readers_writers_TATAS \
        tests/test-and-set tests/test-and-test-and-set tests/test_sem
EXEC = $(FILES:%=%)

all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

## Small tests for Semaphores and Locks

sem: tests/test_sem.c headers/semaphore_interface.h
	$(CC) $(CFLAGS) $< -o tests/$@ $(LIBS)

tas: tests/test-and-set.c headers/TAS.h
	$(CC) $(CFLAGS) $< -o tests/$@ $(LIBS)

tatas: tests/test-and-test-and-set.c headers/TATAS.h
	$(CC) $(CFLAGS) $< -o tests/$@ $(LIBS)

## Main programs

## Philosophers

philosophers: philosophers.c headers/imports.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

philosophers_TAS: philosophers_TAS.c headers/semaphore_interface_TAS.h headers/TAS.h headers/import_TAS.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

philosophers_TATAS: philosophers_TAS.c headers/semaphore_interface_TAS.h headers/TATAS.h headers/import_TATAS.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

## Producers and Consumers

prod_conso: producers_consumers.c headers/imports.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

prod_conso_TAS: prod_conso_TAS.c headers/semaphore_interface_TAS.h headers/TAS.h headers/import_TAS.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

prod_conso_TATAS: prod_conso_TAS.c headers/semaphore_interface_TAS.h headers/TATAS.h headers/import_TATAS.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

## Readers and Writers

readers_writers: readers_writers.c headers/imports.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

readers_writers_TAS: readers_writers_TAS.c headers/semaphore_interface_TAS.h headers/TAS.h headers/import_TAS.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

readers_writers_TATAS: readers_writers_TAS.c headers/semaphore_interface_TAS.h headers/TATAS.h headers/import_TATAS.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(EXEC) *.o
	rm -f tests/sem tests/tas tests/tatas
