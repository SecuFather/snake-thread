#include "snake.h"

void init_snake(Snake *s) {
	s->x[0] = WIDTH/2;
	s->y[0] = HEIGHT/2;
	s->size = 1;
	s->steps = 0;
	s->grow = 0;
}

int is_over(Snake *s, Board *b, char c) {
	return b->field[s->y[0]][s->x[0]] != BG_COLOR ||
			c == 'q' || c == 'Q';
}

int move_snake(Snake *s, Board *b, char c) {
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

	return  (c == 'w' && s->y[0]--) || 
			(c == 's' && s->y[0]++) || 
			(c == 'a' && s->x[0]--) || 
			(c == 'd' && s->x[0]++);
}

void draw_snake(Snake *s, Board *b) {
	int x = s->x[0];
	int y = s->y[0];

	b->field[y][x] = SNAKE_COLOR;
	IN_COLOR(mvprintw(y, x, " "), SNAKE_COLOR);	
}

void start_snake() {
	char c = 0;
	Snake s;
	Board b;

	init_board(&b);
	draw_board(&b);
	init_snake(&s);

	while (!is_over(&s, &b, c)) {
		draw_snake(&s, &b);
		c = getch();
		move_snake(&s, &b, c);
	}
}
