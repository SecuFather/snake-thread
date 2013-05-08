#include "snake.h"

void snake_init(Snake *s, Board *b, int id) {
	s->id = id;
	s->label[0] = 'a'+id;
	s->label[1] = 0;
	board_rand_bg(b, s->x, s->y);
	s->isize = SNAKE_START_SIZE;
	s->size = 1;
	s->steps = 0;
	s->grow = 0;
	s->turn = RIGHT;
	s->alive = ALIVE;
	s->dir  = WEST;
}

int snake_crash(Snake *s, char c, pthread_t *kt) {
	int i;

	if (c == 'q' || c == 'Q') {
		pthread_join(*kt, 0);
		return 1;
	}

	for (i=0; i<SNAKE_COUNT; ++i) {
		if (s[i].alive) {
			return 0;
		}
	}
	return 1;
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
		if (s->size < SNAKE_SIZE) {
			++s->size;
			s->grow = 1;
		}
		s->steps = 0;
	} else {
		if (s->isize) {
			--s->isize;			
			++s->size;
			s->grow = 1;
		}
	}
}

void snake_move(Snake *s, Board *b, Food *f) {
	int i;
	int x = s->x[0];
	int y = s->y[0];

	for (i=s->size-1; i>0; --i) {
		s->x[i] = s->x[i-1];
		s->y[i] = s->y[i-1];
	}


	switch (s->dir) {
		case NORTH:
			y = --s->y[0];
			break;
		case SOUTH:
			y = ++s->y[0];
			break;
		case WEST:
			x = --s->x[0];
			break;
		case EAST:
			x = ++s->x[0];
			break;
	}
	
	b->field[y][x] = SNAKE_COLOR;
}

void snake_draw(Snake *s, Board *b) {
	int x = s->x[0];
	int y = s->y[0];

	IN_COLOR(mvprintw(y, x, s->label), SNAKE_COLOR);	
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

int snake_decide(Snake *s, Board *b, Food *f) {
	int result[4];
	int best = 3;
	int i;

	if (++s->steps > (WIDTH+HEIGHT)/2 && rand_shot(10)) {
		s->turn = -s->turn;	
	}

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
			return s->alive = ALIVE;
		case 1:
			return s->alive = ALIVE;
		case 2:
			s->dir = (s->dir-s->turn+4)%4;
			return s->alive = ALIVE;
	}
	
	return s->alive = DEAD;
}

void snake_destroy(Snake *s, Board *b) {
	int i;
	int x;
	int y;

	for (i=0; i<s->size; ++i) {
		x = s->x[i];
		y = s->y[i];

		b->field[y][x] = BG_COLOR;
		IN_COLOR(mvprintw(y, x, " "), BG_COLOR);
	}
}

void snake_start() {
	char c = 0;
	int i;

	Snake s[SNAKE_COUNT];
	Board b;
	Food f;
	pthread_t kt;
	
	display_init_key_thread(&kt, &c);	
	
	board_init(&b);

	for (i=0; i<SNAKE_COUNT; ++i) {
		snake_init(&s[i],  &b, i);
	}
	food_init(&f, &b);

	while (1) {
		if (snake_crash(s, c, &kt)) {
			break;
		}
		for (i=0; i<SNAKE_COUNT; ++i) {
			if (s[i].alive) {
				snake_draw(&s[i], &b);
			}
		}
		
		refresh();
		usleep(50000);

		for (i=0; i<SNAKE_COUNT; ++i) {
			if (snake_decide(&s[i], &b, &f)) {
				snake_eat_and_grow(&s[i], &b, &f);		
				snake_move(&s[i], &b, &f);
			}
		}
	}
}
