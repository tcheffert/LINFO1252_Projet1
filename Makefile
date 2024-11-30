CC = gcc
CFLAGS = -Wall -Werror
LIBS = -lpthread
FILES = philosophers philosophers_sem producers_consumers prod_conso_sem readers_writers readers_writers_sem test-and-set test-and-test-and-set test_sem
EXEC = $(FILES:%=%)

all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

sem: tests/test_sem.c headers/semaphore_interface.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

philosophers: philosophers.c philosophers_sem.c headers/semaphore_interface.h headers/TAS.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

producers_consumers: producers_consumers.c prod_conso_sem.c headers/semaphore_interface.h headers/TAS.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

readers_writers: readers_writers.c readers_writers_sem.c headers/semaphore_interface.h headers/TAS.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

run: philosophers_sem.c prod_conso_sem.c readers_writers_sem.c headers/semaphore_interface.h headers/TAS.h
	$(CC) $(CFLAGS) philosophers_sem.c -o philosophers $(LIBS)
	$(CC) $(CFLAGS) prod_conso_sem.c -o producers_consumers $(LIBS)
	$(CC) $(CFLAGS) readers_writers_sem.c -o readers_writers $(LIBS)

clean:
	rm -f $(EXEC) $(OBJS)
	rm -f *.o