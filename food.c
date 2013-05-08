#include "food.h"
#include <stdio.h>

void food_init(Food *f, Board *b) {
	food_put(f, b);
}

void food_draw(Food *f) {
	IN_COLOR(mvprintw(f->y, f->x, " "), FOOD_COLOR);	
}

void food_put(Food *f, Board *b) {
	int x, y;

	board_rand_bg(b, &x, &y);

	f->x = x;
	f->y = y;
	b->field[y][x] = FOOD_COLOR;

	food_draw(f);
}
