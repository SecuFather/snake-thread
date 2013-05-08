#include "board.h"

void board_init(Board *b) {
	int i, j;

	for (j=0; j<HEIGHT; ++j) {
		for (i=0; i<WIDTH; ++i) {
			if (i == 0 || j == 0 || i == WIDTH-1 || j == HEIGHT-1) {
				b->field[j][i] = BORDER_COLOR;
			} else {
				if ((abs(WIDTH/2-i) > 5 &&  abs(WIDTH/2-i) < 20 && abs(HEIGHT/2-j) > 1 &&  abs(HEIGHT/2-j) < 7) ||
					(abs(WIDTH/2-i) > 25 &&  abs(WIDTH/2-i) < 30 && abs(HEIGHT/2-j) > 5 &&  abs(HEIGHT/2-j) < 8)){
					b->field[j][i] = BAR_COLOR;
				} else {
					b->field[j][i] = BG_COLOR;
				}
			}
		}
	}

	board_draw(b);
}

void board_draw(Board *b) {
	int i, j;	

	for (j=0; j<HEIGHT; ++j) {
		for (i=0; i<WIDTH; ++i) {
			IN_COLOR(mvprintw(j, i, " "), b->field[j][i]);
		}
	}
}
