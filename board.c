#include "board.h"

void board_init(Board *b) {
	int i, j;

	for (j=0; j<HEIGHT; ++j) {
		for (i=0; i<WIDTH; ++i) {
			if (i == 0 || j == 0 || i == WIDTH-1 || j == HEIGHT-1) {
				b->field[j][i] = BORDER_COLOR;
			} else {
				if ((abs(WIDTH/2-i) > 4 &&  abs(WIDTH/2-i) < 8 && abs(HEIGHT/2-j) > 1 &&  abs(HEIGHT/2-j) < 4) ||
					(abs(WIDTH/2-i) > 25 &&  abs(WIDTH/2-i) < 28 && abs(HEIGHT/2-j) > 5 &&  abs(HEIGHT/2-j) < 8)){
					b->field[j][i] = BAR_COLOR;
				} else {
					b->field[j][i] = BG_COLOR;
				}
			}
		}
	}

	board_draw(b);
	b->score[0] = b->score[1] = 0;
	b->best_score[0] = b->best_score[1] = 0;
}

void board_draw(Board *b) {
	int i, j;	

	for (j=0; j<HEIGHT; ++j) {
		for (i=0; i<WIDTH; ++i) {
			IN_COLOR(mvprintw(j, i, " "), b->field[j][i]);
		}
	}
}

void board_rand_bg(Board *b, int *x, int *y) {
	while(1) {
		*x = rand()%(WIDTH-2)+1;
		*y = rand()%(HEIGHT-2)+1;

		if (pthread_mutex_trylock(&field_mutex[*y][*x]) == 0) {
			if (b->field[*y][*x] == BG_COLOR) {
				break;
			} else {
				pthread_mutex_unlock(&field_mutex[*y][*x]);
			}
		}
	} 
}

void board_show_score(Board *b) {
	char score0[30];
	char score1[30];

	pthread_mutex_lock(&score_mutex);

	sprintf(score0, "Team 0: %d/%d ", b->score[0], b->best_score[0]);
	sprintf(score1, "Team 1: %d/%d ", b->score[1], b->best_score[1]);

	pthread_mutex_unlock(&score_mutex);
	pthread_mutex_lock(&board_mutex);

	IN_COLOR(mvprintw(0, 0, score0), SNAKE_COLOR);
	IN_COLOR(mvprintw(HEIGHT-1, 0, score1), SNAKE_COLOR2);

	pthread_mutex_unlock(&board_mutex);
}

void board_init_mutex() {
	int i, j;

	for (i=0; i<HEIGHT; ++i) {
		for (j=0; j<WIDTH; ++j) {
				pthread_mutex_init(&field_mutex[i][j], NULL);
		}
	}
	pthread_mutex_init(&board_mutex, NULL);
	pthread_mutex_init(&score_mutex, NULL);
	pthread_mutex_init(&pause_mutex, NULL);
}

void board_finalize_mutex() {
	int i, j;

	for (i=0; i<HEIGHT; ++i) {
		for (j=0; j<WIDTH; ++j) {
				pthread_mutex_destroy(&field_mutex[i][j]);
		}
	}
	pthread_mutex_destroy(&board_mutex);
	pthread_mutex_destroy(&score_mutex);
	pthread_mutex_destroy(&pause_mutex);
}

void board_set_field(Board *b, int x, int y, int v) {
	b->field[y][x] = v;
	pthread_mutex_unlock(&field_mutex[y][x]);
}
