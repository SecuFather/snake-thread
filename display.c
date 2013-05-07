#include "display.h"

void init_colors() {
	init_pair(BORDER_COLOR, COLOR_BLACK, COLOR_WHITE);
	init_pair(BG_COLOR, COLOR_BLACK, COLOR_BLACK);
	init_pair(SNAKE_COLOR, COLOR_BLACK, COLOR_GREEN);
}

void init_display() {
	initscr();
	noecho();
	cbreak();

	curs_set(0);
	start_color();
	init_colors();
}
