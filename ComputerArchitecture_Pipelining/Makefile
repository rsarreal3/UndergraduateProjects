simulator: simulator.c data_struct.c pipelining.c memory.c
	gcc -Wall -o simulator simulator.c memory.c pipelining.c data_struct.c



clean:
	rm -rf *.o *~ *#

val:
	valgrind ./simulator $(FILENAME)

run:
	./simulator $(FILENAME)
