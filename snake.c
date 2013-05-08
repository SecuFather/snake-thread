#include "snake.h"

void snake_init(Snake *s) {
	s->x[0] = WIDTH/2;
	s->y[0] = HEIGHT/2;
	s->size = 1;
	s->steps = 0;
	s->grow = 0;
}

int snake_crash(Snake *s, Board *b, char c) {
	return b->field[s->y[0]][s->x[0]] != BG_COLOR ||
			c == 'q' || c == 'Q';
}

int snake_move(Snake *s, Board *b, char c) {
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

void snake_draw(Snake *s, Board *b) {
	int x = s->x[0];
	int y = s->y[0];

	b->field[y][x] = SNAKE_COLOR;
	IN_COLOR(mvprintw(y, x, " "), SNAKE_COLOR);	
}

void snake_start() {
	char c = 0;
	Snake s;
	Board b;

	board_init(&b);
	board_draw(&b);
	snake_init(&s);

	while (!snake_crash(&s, &b, c)) {
		snake_draw(&s, &b);
		c = getch();
		snake_move(&s, &b, c);
	}
}
