CC = gcc
CFLAGS = -Wall -std=c99

proj3: main.o threat.o map.o game.o
	$(CC) $(CFLAGS) main.o threat.o map.o game.o -o proj3


main.o:  main.c
	$(CC) $(CFLAGS) -c  main.c -o main.o

threat.o: threat.h threat.c
	$(CC) $(CFLAGS) -c threat.c -o threat.o

map.o: map.h map.c threat.h threat.c
	$(CC) $(CFLAGS) -c map.c -o map.o

game.o: game.h game.c
	$(CC) $(CFLAGS) -c game.c -o game.o



clean:
	$(RM) -rf *.o *~

val: 
	valgrind ./proj3 $(FILENAME)

run:
	./proj3 $(FILENAME)

# -c means link in the object file, dont make an executable :)