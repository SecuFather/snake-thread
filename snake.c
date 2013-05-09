#include "snake.h"

void snake_init(Snake *s, Board *b, int id) {
	s->id = id;
	s->label[0] = 'a'+id;
	s->label[1] = 0;
	board_rand_bg(b, s->x, s->y);
	s->isize = SNAKE_START_SIZE;
	s->size = 0;
	s->steps = 0;
	s->turn = RIGHT;
	s->alive = ALIVE;
	s->dir  = WEST;
	s->team = (id >= SNAKE_COUNT/2);
	s->color = (s->team ? SNAKE_COLOR : SNAKE_COLOR2);
	b->field[s->y[0]][s->x[0]] = s->color;
	s->grow = 0;

	snake_change_size(s, b, 1);
}

int snake_crash(Snake *s, char c, pthread_t *kt) {
	if (c == 'q' || c == 'Q') {
		display_add_log("przerwanie programu przez użytkownika...");
		pthread_join(*kt, 0);
		finish = 1;
		return 1;
	}

	return 0;
}

void snake_eat_and_grow(Snake *s, Board *b, Food *f) {
	int x = s->x[0];
	int y = s->y[0];

	if (f->y == y && f->x == x) {
		food_put(f, b);
		s->grow = 1;
	} else {
		if (s->isize) {
			--s->isize;			
			s->grow = 1;
		}
	}
}

void snake_move(Snake *s, Board *b, Food *f) {
	int i;
	int x = s->x[0];
	int y = s->y[0];
	int tx = s->x[s->size-1];
	int ty = s->y[s->size-1];

	if (s->grow) {
		snake_change_size(s, b, 1);
		s->grow = 0;
	}

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
	
	b->field[y][x] = s->color;
	IN_COLOR(mvprintw(y, x, s->label), s->color);
	b->field[ty][tx] = BG_COLOR;
	IN_COLOR(mvprintw(ty, tx, " "), BG_COLOR);
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

	if (++s->steps > (WIDTH+HEIGHT)*SNAKE_COUNT/4) {
		s->turn = -s->turn;	
		if (snake_change_size(s, b, -1)) {
			b->field[s->y[s->size]][s->x[s->size]] = BG_COLOR;
			IN_COLOR(mvprintw(s->y[s->size], s->x[s->size], " "), BG_COLOR);
		}
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
	snake_reverse(s);
	return s->alive = DEAD;
}

int snake_change_size(Snake *s, Board *b, int x) {
	s->size += x;
	b->score[s->team] += x;
	s->steps = 0;

	if (b->score[s->team] > b->best_score[s->team]) {
		b->best_score[s->team] = b->score[s->team];
	}
	if (s->size < 1) {
		s->size = 1;
		return 0;
	}
	if (s->size == SNAKE_SIZE) {
		s->size = SNAKE_SIZE-1;
		return 0;
	}

	return 1;
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

void snake_reverse(Snake *s) {
	int i, i2;
	int n = s->size;

	for (i=0; i<n/2; ++i) {
		i2 = n-i-1;

		swap(&s->y[i], &s->y[i2]);
		swap(&s->x[i], &s->x[i2]);
	}
}

void *snake_thread(void *x) {
	int id = (int)x;

	pthread_mutex_lock(&mutex);
	snake_init(&s[id], &b, id);
	pthread_mutex_unlock(&mutex);

	while(!finish) {
		pthread_mutex_lock(&mutex);

		snake_eat_and_grow(&s[id], &b, &f);		
		if (snake_decide(&s[id], &b, &f)) {
			snake_move(&s[id], &b, &f);
		}
		board_show_score(&b);
		refresh();
		pthread_mutex_unlock(&mutex);
		
		usleep(SNAKE_DELAY);
	}	
	pthread_exit(NULL);
}


void snake_start() {
	char c = 0;
	int i;
	finish = 0;

	pthread_t kt, st[SNAKE_COUNT];
	
	display_add_log("startowanie wątku klawiatury...");
	pthread_create(&kt, NULL, display_getch, (void *) &c);	

	display_add_log("inicjacja mutexu...");
	pthread_mutex_init(&mutex, NULL);
	
	display_add_log("inicjacja planszy i jedzenia...");
	board_init(&b);
	food_init(&f, &b);

	display_add_log("startowanie węży...");
	for (i=0; i<SNAKE_COUNT; ++i) {
		pthread_create(&st[i], NULL, snake_thread, (void *) i);
	}

	display_add_log("wyświetlanie węży...");
	while (!snake_crash(s, c, &kt)) {		
		usleep(SNAKE_DELAY);
	}

	display_add_log("kończenie wątków węży...");
	for (i=0; i<SNAKE_COUNT; ++i) {
		pthread_join(st[i], NULL);
	}

	display_add_log("niszczenie mutexu...");
	pthread_mutex_destroy(&mutex);
}
