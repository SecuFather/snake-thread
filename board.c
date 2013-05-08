#include "board.h"

void board_init(Board *b) {
	int i, j;

	for (j=0; j<HEIGHT; ++j) {
		for (i=0; i<WIDTH; ++i) {
			if (i == 0 || j == 0 || i == WIDTH-1 || j == HEIGHT-1) {
				b->field[j][i] = BORDER_COLOR;
			} else {
				b->field[j][i] = BG_COLOR;
			}
		}
	}
}

void board_draw(Board *b) {
	int i, j;	

	for (j=0; j<HEIGHT; ++j) {
		for (i=0; i<WIDTH; ++i) {
			IN_COLOR(mvprintw(j, i, " "), b->field[j][i]);
		}
	}
}
