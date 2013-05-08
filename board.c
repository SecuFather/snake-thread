#include "board.h"

void board_init(Board *b) {
	int i, j;

	for (j=0; j<HEIGHT; ++j) {
		for (i=0; i<WIDTH; ++i) {
			if (i == 0 || j == 0 || i == WIDTH-1 || j == HEIGHT-1) {
				b->field[j][i] = BORDER_COLOR;
			} else {
				if (((i-WIDTH/4)*(i-WIDTH/4)/2+(j-HEIGHT/4)*(j-HEIGHT/4) < 9) ||
					((i-3*WIDTH/4)*(i-3*WIDTH/4)/2+(j-3*HEIGHT/4-1)*(j-3*HEIGHT/4-1) < 9)) {
					b->field[j][i] = BAR_COLOR;
				} else {
					b->field[j][i] = BG_COLOR;
				}
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
