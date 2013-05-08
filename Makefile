CC=gcc
CFLAGS=-Wall -lncurses -c -g
LDFLAGS=-lncurses
SOURCES=display.c snake.c main.c board.c
OBJECTS=$(SOURCES:.c=.o)
INCLUDES=$(SOURCES:.c=.h)
EXECUTABLE=snake

all: $(SOURCES) $(EXECUTABLE) run

$(EXECUTABLE):	$(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:	$(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

run:	$(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)


