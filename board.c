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
	do {
		*x = rand()%(WIDTH-2)+1;
		*y = rand()%(HEIGHT-2)+1;

	} while(b->field[*y][*x] != BG_COLOR);
}

void board_show_score(Board *b) {
	char score[10];

	sprintf(score, "Team 0: %d/%d ", b->score[0], b->best_score[0]);
	IN_COLOR(mvprintw(0, 0, score), SNAKE_COLOR);
	sprintf(score, "Team 1: %d/%d ", b->score[1], b->best_score[1]);
	IN_COLOR(mvprintw(HEIGHT-1, 0, score), SNAKE_COLOR2);
}
