#include "snake.h"

void snake_init(Snake *s, Board *b, int id) {
	s->id = id;
	s->label[0] = 'a'+id;
	s->label[1] = 0;
	s->isize = SNAKE_START_SIZE;
	s->size = 0;
	s->steps = 0;
	s->turn = RIGHT;
	s->alive = ALIVE;
	s->dir  = WEST;
	s->team = (id >= SNAKE_COUNT/2);
	s->color = (s->team ? SNAKE_COLOR2 : SNAKE_COLOR);
	s->grow = 0;

	board_rand_bg(b, s->x, s->y);
	board_set_field(b, s->x[0], s->y[0], s->color);	
	snake_change_size(s, b, 1);
}

int snake_crash(Snake *s, char *c, pthread_t *kt) {
	if (*c == 'q' || *c == 'Q') {
		display_add_log("przerwanie programu przez użytkownika...");
		pthread_join(*kt, 0);
		snake_finish = 1;

		if (snake_pause) {
			pthread_cond_broadcast(&step_cond);
		}
		return 1;
	}

	if (*c == 'p' || *c == 'P') {
		snake_pause = 1;
	}

	if (*c == 'r' || *c == 'R') {
		if (snake_pause) {
			snake_pause = 0;
			pthread_cond_broadcast(&step_cond);
		}
	}

	if (*c == 's' || *c == 'S') {
		if (snake_pause) {		
			*c = 0;	
			pthread_cond_signal(&step_cond);
		}
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
	int tx;
	int ty;

	if (s->grow) {
		snake_change_size(s, b, 1);
		s->grow = 0;
	}

	tx = s->x[s->size-1];
	ty = s->y[s->size-1];

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

	board_set_field(b, x, y, s->color);
	b->field[ty][tx] = BG_COLOR;

	pthread_mutex_lock(&board_mutex);

	IN_COLOR(mvprintw(y, x, s->label), s->color);
	IN_COLOR(mvprintw(ty, tx, " "), BG_COLOR);

	pthread_mutex_unlock(&board_mutex);
}

int snake_check_head(Snake *snake, Board *b) {
	int i;
	int ib = (snake->team ? SNAKE_COUNT/2 : 0);
	int ie = (snake->team ? SNAKE_COUNT : SNAKE_COUNT/2);	
	int x = snake->x[0];
	int y = snake->y[0];	
	int sx, sy;

	for (i=ib; i<ie; ++i) {
		if (i != snake->id) {
			if (s[i].alive == ALIVE && pthread_mutex_trylock(&snake_mutex[i]) == 0) {
				sx = s[i].x[0];
				sy = s[i].y[0];

				if ((sx == x && sy == y-1) || (sx == x && sy == y+1) || 
					(sx == x+1 && sy == y) || (sx == x-1 && sy == y)) {
					snake->steps = 1;

					pthread_mutex_unlock(&snake_mutex[i]);
					return 1;
				}
				pthread_mutex_unlock(&snake_mutex[i]);
			}
		}
	}
	return 0;
}

int snake_check_tail(Snake *snake, Board *b, int x, int y) {
	int i;
	int ib = (snake->team ? 0 : SNAKE_COUNT/2);
	int ie = (snake->team ? SNAKE_COUNT/2 : SNAKE_COUNT);
	int last;

	for (i=ib; i<ie; ++i) {
		if (i != snake->id) {
			if (s[i].alive == ALIVE && pthread_mutex_trylock(&snake_mutex[i]) == 0) {
				last = s[i].size-1;

				if (s[i].x[last] == x && s[i].y[last] == y) {
					snake_change_size(&s[i], b, -1);
					snake->grow = 1;
					pthread_mutex_unlock(&snake_mutex[i]);
					return 1;
				}

				pthread_mutex_unlock(&snake_mutex[i]);
			}
		}
	}

	return 0;
}

int snake_rate_direction(Snake *s, Board *b, Food *f, int x, int y, int fd, pthread_mutex_t **m, int *best, int *best_result, int id) {
	int v = b->field[y][x];
	int r;
	
	if (*best_result == 4) {
		return 0;
	}

	if (pthread_mutex_trylock(&field_mutex[y][x]) == 0) {
		r = v <= BG_COLOR ? 
			fd+1 : 
			(v == (s->team ? SNAKE_COLOR : SNAKE_COLOR2) ? 
				snake_check_tail(s, b, x, y)*4 : 
				0);

		if (r > *best_result) {
			if (*m != 0) {
				pthread_mutex_unlock(*m);
			}
			*best = id;
			*best_result = r;

			*m = &field_mutex[y][x];			
		} else {
			pthread_mutex_unlock(&field_mutex[y][x]);
		}

		return r;
	} 

	return 0;
}

int snake_check_direction(Snake *s, Board *b, Food *f, char dir, pthread_mutex_t **m, int *best, int *best_result, int id) {
	int x = s->x[0];
	int y = s->y[0];

	switch (dir) {
		case NORTH:
			return snake_rate_direction(s, b, f, x, y-1, f->y < y, m, best, best_result, id);
		case EAST:
			return snake_rate_direction(s, b, f, x+1, y, f->x > x, m, best, best_result, id);
		case SOUTH:
			return snake_rate_direction(s, b, f, x, y+1, f->y > y, m, best, best_result, id);
		case WEST:
			return snake_rate_direction(s, b, f, x-1, y, f->x < x, m, best, best_result, id);
	}

	return 0;
}

void snake_cut_tail(Snake *s, Board *b) {
	int x = s->x[s->size];
	int y = s->y[s->size];

	pthread_mutex_lock(&field_mutex[y][x]);
	board_set_field(b, x, y, BG_COLOR);

	pthread_mutex_lock(&board_mutex);
	IN_COLOR(mvprintw(y, x, " "), BG_COLOR);
	pthread_mutex_unlock(&board_mutex);
}

int snake_decide(Snake *s, Board *b, Food *f) {
	int best = 3;
	int best_result = 0;
	int x, y;
	pthread_mutex_t *m = 0;

	snake_check_head(s, b);

	if (++s->steps > (WIDTH+HEIGHT)*SNAKE_COUNT/8) {
		s->turn = -s->turn;	
		if (snake_change_size(s, b, -1)) {
			snake_cut_tail(s, b);

			if (s->alive == DEAD) {
				pthread_exit(NULL);
			}
		}
	}

	x = s->x[0];
	y = s->y[0];

	snake_check_direction(s, b, f, pos_mod(s->dir+s->turn, 4), &m, &best, &best_result, 0);	
	snake_check_direction(s, b, f, s->dir, &m, &best, &best_result, 1);
	snake_check_direction(s, b, f, pos_mod(s->dir-s->turn, 4), &m, &best, &best_result, 2);	

	switch (best) {
		case 0:
			s->dir = (s->dir+s->turn+4)%4;
			s->turn = -s->turn;	
			break;
		case 1:
			break;
		case 2:
			s->dir = (s->dir-s->turn+4)%4;
			break;
	}

	if (best < 3) {
		return 1;
	}

	snake_reverse(s);

	return 0;
}

int snake_change_size(Snake *s, Board *b, int x) {
	s->size += x;
	s->steps = 0;

	pthread_mutex_lock(&score_mutex);
	b->score[s->team] += x;

	if (b->score[s->team] > b->best_score[s->team]) {
		b->best_score[s->team] = b->score[s->team];
	}
	pthread_mutex_unlock(&score_mutex);

	if (s->size < 1) {
		s->alive = DEAD;
		return 1;
	}
	if (s->size == SNAKE_SIZE) {
		s->size = SNAKE_SIZE-1;
		return 0;
	}

	return 1;
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

	snake_init(&s[id], &b, id);

	while(!snake_finish) {		
		if (snake_pause) {
			pthread_cond_wait(&step_cond, &step_mutex);			
			pthread_mutex_unlock(&step_mutex);
		}

		pthread_mutex_lock(&snake_mutex[id]);

		if (s[id].alive != DEAD) {
			snake_eat_and_grow(&s[id], &b, &f);		
			if (snake_decide(&s[id], &b, &f)) {
				snake_move(&s[id], &b, &f);
			}
		} else {
			pthread_exit(NULL);
		}

		pthread_mutex_unlock(&snake_mutex[id]);

		board_show_score(&b);

		pthread_mutex_lock(&board_mutex);
		refresh();
		pthread_mutex_unlock(&board_mutex);

		usleep(SNAKE_DELAY);
	}	
	pthread_exit(NULL);
}


void snake_start() {
	char c = 0;
	int i;
	snake_finish = 0;
	snake_pause = 0;
	snake_step = 0;

	pthread_t kt, st[SNAKE_COUNT];
	
	display_add_log("startowanie wątku klawiatury...");
	pthread_create(&kt, NULL, display_getch, (void *) &c);	
	
	display_add_log("inicjacja mutexow...");
	board_init_mutex();
	snake_init_mutex();

	display_add_log("inicjacja planszy...");
	board_init(&b);

	display_add_log("inicjacja jedzenia...");
	food_init(&f, &b);

	display_add_log("startowanie węży...");
	for (i=0; i<SNAKE_COUNT; ++i) {
		pthread_create(&st[i], NULL, snake_thread, (void *) i);
	}

	display_add_log("wyświetlanie węży...");
	while (!snake_crash(s, &c, &kt)) {		
		usleep(SNAKE_DELAY);
	}

	display_add_log("kończenie wątków węży...");
	for (i=0; i<SNAKE_COUNT; ++i) {
		pthread_join(st[i], NULL);
	}

	display_add_log("niszczenie mutexow...");
	board_finalize_mutex();
	snake_finalize_mutex();
}

void snake_init_mutex() {
	int i;
	
	for (i=0; i<SNAKE_COUNT; ++i) {
		pthread_mutex_init(&snake_mutex[i], NULL);
	}
	pthread_mutex_init(&step_mutex, NULL);
	pthread_cond_init(&step_cond, NULL);
}

void snake_finalize_mutex() {
	int i;
	
	for (i=0; i<SNAKE_COUNT; ++i) {
		pthread_mutex_destroy(&snake_mutex[i]);
	}
	pthread_mutex_destroy(&step_mutex);
	pthread_cond_destroy(&step_cond);
}
