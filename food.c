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
	board_set_field(b, x, y, FOOD_COLOR);

	pthread_mutex_lock(&board_mutex);
	food_draw(f);
	pthread_mutex_unlock(&board_mutex);
}
