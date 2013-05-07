#include "snake.h"

void init_snake(Snake *s) {
	s->x[0] = WIDTH/2;
	s->y[0] = HEIGHT/2;
	s->size = 1;
	s->steps = 0;
	s->grow = 0;

	IN_COLOR(draw_board(), BORDER_COLOR);
}

int is_over(Snake *s, char c) {
	return s->x[0] == 0 || s->x[0] == WIDTH || s->y[0] == 0 || s->y[0] == HEIGHT ||
			c == 'q' || c == 'Q';
}

int move_snake(Snake *s, char c) {
	int i;

	if (!s->grow) {
		IN_COLOR(mvprintw(s->y[s->size-1], s->x[s->size-1], " "), BG_COLOR);	
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

void draw_snake(Snake *s) {
	IN_COLOR(mvprintw(s->y[0], s->x[0], " "), SNAKE_COLOR);	
}

void start_snake() {
	char c = 0;
	Snake s;

	init_snake(&s);

	while (!is_over(&s, c)) {
		draw_snake(&s);
		c = getch();
		move_snake(&s, c);
	}
}
