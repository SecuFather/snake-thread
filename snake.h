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
void init_snake(Snake *s);

//sprawdza czy nastąpiły warunki do zakończenia programu
int is_over(Snake *s, char c);

//porusza wężem
int move_snake(Snake *s, char c);

//rysuje węża
void draw_snake(Snake *s);

//startuje węża
void start_snake();

#endif
