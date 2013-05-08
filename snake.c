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

int snake_move(Snake *s, Board *b) {
	int i;
	int tx = s->x[s->size-1];
	int ty = s->y[s->size-1];

	if (!s->grow) {
		b->field[ty][tx] = BG_COLOR;
		IN_COLOR(mvprintw(ty, tx, " "), BG_COLOR);	
	} else {
		s->grow = 0;
	}

	for (i=s->size-1; i>0; --i) {
		s->x[i] = s->x[i-1];
		s->y[i] = s->y[i-1];
	}

	if (++s->steps % 10 == 0) {
		++s->size;
		s->grow = 1;
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

int snake_check_direction(Snake *s, Board *b, char dir) {
	int x = s->x[0];
	int y = s->y[0];

	switch (dir) {
		case NORTH:
			return b->field[--y][x] == BG_COLOR;
		case EAST:
			return b->field[y][++x] == BG_COLOR;
		case SOUTH:
			return b->field[++y][x] == BG_COLOR;
		case WEST:
			return b->field[y][--x] == BG_COLOR;
	}
	return 0;
}

void snake_decide(Snake *s, Board *b) {
	if (snake_check_direction(s, b, s->dir)) {
		return;
	} else {
		if (snake_check_direction(s, b, (s->dir+s->turn+4)%4)) {
			s->dir = (s->dir+s->turn+4)%4;
			s->turn = -s->turn;			
		} else {
			if (snake_check_direction(s, b, (s->dir-s->turn+4)%4)) {
				s->dir = (s->dir-s->turn+4)%4;
			} else {
				s->alive = DEAD;
			}
		}
	}
}

void snake_start() {
	char c = 0;
	Snake s;
	Board b;

	board_init(&b);
	board_draw(&b);
	snake_init(&s);

	while (!snake_crash(&s, c)) {
		snake_draw(&s, &b);
		snake_decide(&s, &b);
		c = getch();
		snake_move(&s, &b);
	}
}
