CC=gcc
LIBS=-pthread -lncurses
CFLAGS=-Wall -c -g
LDFLAGS=
SOURCES=global.c display.c snake.c main.c board.c food.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=snake

all: clean $(SOURCES) $(EXECUTABLE) run

$(EXECUTABLE):	$(OBJECTS)
	$(CC) $(LDFLAGS) $(LIBS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

run:	$(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)


