
# A Makefile to build project 2

C99     =  cc -std=c99
CFLAGS  =  -Wall -pedantic -Werror


mergetars : mergetars.o processes.o files.o 
	$(C99) $(CFLAGS) -o mergetars \
		mergetars.o processes.o files.o -lm


mergetars.o : mergetars.c mergetars.h
	$(C99) $(CFLAGS) -c mergetars.c

processes.o : processes.c mergetars.h
	$(C99) $(CFLAGS) -c processes.c

files.o : files.c mergetars.h
	$(C99) $(CFLAGS) -c files.c

clean:
	rm -rf mergetars *.o
