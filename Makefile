CFLAGS = -std=c99

gotchi: main.c checkfiles.o
	gcc $(CFLAGS) -o gotchi main.c checkfiles.o

checkfiles.o: checkfiles.h checkfiles.c
	gcc $(CFLAGS) -c checkfiles.c

clean:
	rm -f *.o
