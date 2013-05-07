#include "board.h"

void draw_board() {
	int i;	

	for (i=0; i<=WIDTH; ++i) {
		mvprintw(0, i, " ");
		mvprintw(HEIGHT, i, " ");
	}
	for (i=0; i<=HEIGHT; ++i) {
		mvprintw(i, 0, " ");
		mvprintw(i, WIDTH, " ");
	}
}
