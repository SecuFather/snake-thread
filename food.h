#ifndef H_FOOD_H
#define H_FOOD_H

#include "board.h"

typedef struct {
	int x, y;
} Food;

//inicjuje jedzenie
void food_init(Food *f, Board *b);

//rysuje jedzenie
void food_draw(Food *f);

//wyznacza losowo nowe położenie jedzenia
void food_put(Food *f, Board *b);

#endif
