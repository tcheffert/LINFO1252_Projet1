CC = gcc
CFLAGS = -Wall -Werror -pthread -g
LIBS = -lpthread

FILES = philosophers philosophers_TAS philosophers_TATAS \
		producers_consumers prod_conso_TAS prod_conso_TATAS \
        readers_writers readers_writers_TAS readers_writers_TATAS \
        tests/test-and-set tests/test-and-test-and-set tests/test_sem

DEPENDENCIES = headers/imports.h headers/imports_TAS.h headers/imports_TATAS.h \
		headers/semaphore_interface.h headers/semaphore_interface_TAS.h headers/semaphore_interface_TATAS.h \
		headers/TAS.h headers/TATAS.h

EXEC = $(FILES:%=%)

PYTHON_SCRIPT = multiple_plots.py

all: $(DEPENDENCIES) $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

%: %.o
	$(CC) $< -o $@ $(LIBS)

## Small tests for Semaphores and Locks
sem: tests/test_sem.c headers/semaphore_interface.h
	$(CC) $(CFLAGS) $< -o tests/$@ $(LIBS)

tas: tests/test-and-set.c headers/TAS.h
	$(CC) $(CFLAGS) $< -o tests/$@ $(LIBS)

tatas: tests/test-and-test-and-set.c headers/TATAS.h
	$(CC) $(CFLAGS) $< -o tests/$@ $(LIBS)

test: sem tas tatas
	@echo "-------------------------------------------------------------------------------------"
	@echo "All test targets have been built."
	@echo "You can go in the \033[1;33mtests\033[0m file to run them with the desired number of threads: \033[1;32mcd tests/\033[0m "

## Main programs

## Philosophers

# philosophers: philosophers.c headers/imports.h
# 	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

# philosophers_TAS: philosophers_TAS.c headers/semaphore_interface_TAS.h headers/TAS.h headers/imports_TAS.h
# 	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

# philosophers_TATAS: philosophers_TATAS.c headers/semaphore_interface_TATAS.h headers/TATAS.h headers/imports_TATAS.h
# 	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

## Producers and Consumers

prod_conso: producers_consumers.c headers/imports.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

prod_conso_TAS: prod_conso_TAS.c headers/semaphore_interface_TAS.h headers/TAS.h headers/imports_TAS.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

prod_conso_TATAS: prod_conso_TATAS.c headers/semaphore_interface_TATAS.h headers/TATAS.h headers/imports_TATAS.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

## Readers and Writers

readers_writers: readers_writers.c headers/imports.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

readers_writers_TAS: readers_writers_TAS.c headers/semaphore_interface_TAS.h headers/TAS.h headers/imports_TAS.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

readers_writers_TATAS: readers_writers_TATAS.c headers/semaphore_interface_TATAS.h headers/TATAS.h headers/imports_TATAS.h
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(EXEC) *.o
	rm -f tests/sem tests/tas tests/tatas

# Plot the graphs
.PHONY: plot
plot:
	@echo "Running Python script to display the plots..."
	python3 $(PYTHON_SCRIPT)

help:
	@echo "============================================================"
	@echo "                         \033[1;33mMAKE COMMANDS\033[0m                     "
	@echo "============================================================"
	@echo ""
	@echo "General Commands:"
	@echo "  \033[1;32mall\033[0m           - Build all main executables and dependencies."
	@echo "  \033[1;32mclean\033[0m         - Remove all built files and object files."
	@echo "  \033[1;32mtest\033[0m          - Bulid all the tests in the \033[1;31mtests\033[0m file. Go the the file to run them."
	@echo "  \033[1;32mplot\033[0m          - Run the Python script to generate and display plots."
	@echo ""
	@echo "------------------------------------------------------------"
	@echo "Specific Program Commands:"
	@echo "  \033[1;34msem\033[0m               	- Build semaphore interface test executable (2.4)."
	@echo "  \033[1;34mtas\033[0m               	- Build test-and-set lock test executable (2.2)."
	@echo "  \033[1;34mtatas\033[0m             	- Build test-and-test-and-set lock test executable (2.3)."
	@echo "  \033[1;34mprod_conso\033[0m        	- Build the producers and consumers program."
	@echo "  \033[1;34mprod_conso_TAS\033[0m    	- Build the Test-And-Set version of the producers and consumers program."
	@echo "  \033[1;34mprod_conso_TATAS\033[0m  	- Build the Test-And-Test-And-Set version of the producers and consumers program."
	@echo "  \033[1;34mreaders_writers\033[0m   	- Build the readers and writers program."
	@echo "  \033[1;34mreaders_writers_TAS\033[0m   - Build the Test-And-Set version of the readers and writers program."
	@echo "  \033[1;34mreaders_writers_TATAS\033[0m - Build the Test-And-Test-And-Set version of the readers and writers program."
	@echo ""
	@echo "============================================================"
	@echo "  Use '\033[1;33mmake <command>\033[0m' to execute a specific target."
	@echo "============================================================"
