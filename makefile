#compiler: 
CC = gcc 
#compiler flags:
#-g -ansi -Wall - pedantic -std=gnu99 -I -Wno-missing-braces
CFLAGS = -g -Wall -std=gnu99 
OBJECTS = keygen

all: $(OBJECTS)

$(OBJECTS): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

#input_handling: input_handling.c
#	$(CC) $(CFLAGS) -o input_handling.o input_handling.c

#built_in_cmnds: built_in_cmnds.c
#	$(CC) $(CFLAGS) -o built_in_cmnds.o built_in_cmnds.c

#exec_cmnds: exec_cmnds.c
#	$(CC) $(CFLAGS) -o exec_cmnds.o exec_cmnds.c

clean:
	$(RM) keygen *.o *~