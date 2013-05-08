#ifndef H_SNAKE_H
#define H_SNAKE_H

#include "global.h"
#include "board.h"

#define SNAKE_SIZE		100

typedef struct {
	int x[SNAKE_SIZE];
	int y[SNAKE_SIZE];
	int steps;
	int size;
	int grow;
} Snake;

//inicjuje węża
void snake_init(Snake *s);

//sprawdza czy nastąpiły warunki do zakończenia programu
int snake_crash(Snake *s, Board *b, char c);

//porusza wężem
int snake_move(Snake *s, Board *b, char c);

//rysuje węża
void snake_draw(Snake *s, Board *b);

//startuje węża
void snake_start();

#endif
