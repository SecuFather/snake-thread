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

	do {
		x = rand()%(WIDTH-2)+1;
		y = rand()%(HEIGHT-2)+1;

	} while(b->field[y][x] != BG_COLOR);

	f->x = x;
	f->y = y;
	b->field[y][x] = FOOD_COLOR;

	food_draw(f);
}
