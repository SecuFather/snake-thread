#include "snake.h"

void snake_init(Snake *s) {
	s->x[0] = WIDTH/2;
	s->y[0] = HEIGHT/2;
	s->size = 1;
	s->steps = 0;
	s->grow = 0;
	s->turn = RIGHT;
	s->alive = ALIVE;
	s->dir  = WEST;
}

int snake_crash(Snake *s, char c) {
	return c == 'q' || c == 'Q' || !s->alive;
}

void snake_eat_and_grow(Snake *s, Board *b, Food *f) {
	int x = s->x[0];
	int y = s->y[0];
	int tx = s->x[s->size-1];
	int ty = s->y[s->size-1];

	if (!s->grow) {
		b->field[ty][tx] = BG_COLOR;
		IN_COLOR(mvprintw(ty, tx, " "), BG_COLOR);
	} else {
		s->grow = 0;
	}	

	if (f->y == y && f->x == x) {
		food_put(f, b);
		++s->size;
		s->grow = 1;
	}
}

int snake_move(Snake *s, Board *b, Food *f) {
	int i;

	for (i=s->size-1; i>0; --i) {
		s->x[i] = s->x[i-1];
		s->y[i] = s->y[i-1];
	}

	return  (s->dir == NORTH && s->y[0]--) || 
			(s->dir == SOUTH && s->y[0]++) || 
			(s->dir == WEST && s->x[0]--) || 
			(s->dir == EAST && s->x[0]++);
}

void snake_draw(Snake *s, Board *b) {
	int x = s->x[0];
	int y = s->y[0];

	b->field[y][x] = SNAKE_COLOR;
	IN_COLOR(mvprintw(y, x, " "), SNAKE_COLOR);	
}

int snake_check_direction(Snake *s, Board *b, Food *f, char dir) {
	int x = s->x[0];
	int y = s->y[0];

	switch (dir) {
		case NORTH:
			return b->field[--y][x] <= BG_COLOR ? (f->y <= y)+1 : 0;
		case EAST:
			return b->field[y][++x] <= BG_COLOR ? (f->x >= x)+1 : 0;
		case SOUTH:
			return b->field[++y][x] <= BG_COLOR ? (f->y >= y)+1 : 0;
		case WEST:
			return b->field[y][--x] <= BG_COLOR ? (f->x <= x)+1 : 0;
	}

	return 0;
}

void snake_decide(Snake *s, Board *b, Food *f) {
	int result[4];
	int best = 3;
	int i;

	result[0] = snake_check_direction(s, b, f, pos_mod(s->dir+s->turn, 4));
	result[1] = snake_check_direction(s, b, f, s->dir);
	result[2] = snake_check_direction(s, b, f, pos_mod(s->dir-s->turn, 4));
	result[3] = 0;

	for (i=0; i<3; ++i) {
		if (result[i] > result[3]) {
			best = i;
			result[3] = result[i];
		}
	}

	switch (best) {
		case 0:
			s->dir = (s->dir+s->turn+4)%4;
			s->turn = -s->turn;	
			return;
		case 1:
			return;
		case 2:
			s->dir = (s->dir-s->turn+4)%4;
			return;
	}
	s->alive = DEAD;
}

void snake_start() {
	char c = 0;
	Snake s;
	Board b;
	Food f;

	board_init(&b);
	snake_init(&s);
	food_init(&f, &b);

	while (!snake_crash(&s, c)) {
		snake_draw(&s, &b);
		snake_decide(&s, &b, &f);

		c = getch();

		snake_eat_and_grow(&s, &b, &f);		
		snake_move(&s, &b, &f);
	}
}
