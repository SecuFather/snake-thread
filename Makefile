CC=gcc
LIBS=-pthread -lncurses
CFLAGS=-Wall -c
LDFLAGS=
SOURCES=global.c display.c snake.c main.c board.c food.c
OBJECTS=$(SOURCES:.c=.o)
INCLUDES=$(SOURCES:.c=.h)
EXECUTABLE=snake

all: $(SOURCES) $(EXECUTABLE) run

$(EXECUTABLE):	$(OBJECTS)
	$(CC) $(LDFLAGS) $(LIBS) $(OBJECTS) -o $@

.c.o:	$(INCLUDES)
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

run:	$(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)


