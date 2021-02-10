CFLAGS=-g -W -Wall
PROGRAMS=sgrep


sgrep:	sgrep.o
	gcc -o sgrep $^
	
sgrep.o:sgrep.c	

clean:
	rm*.o
