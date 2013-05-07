#ifndef H_GLOBAL_H
#define H_GLOBAL_H

#include <ncurses.h>

#define BORDER_COLOR 	1
#define BG_COLOR  		2
#define SNAKE_COLOR		3
#define WIDTH			80
#define HEIGHT			20
#define IN_COLOR(function, color) attron(COLOR_PAIR(color)); function; attroff(COLOR_PAIR(color))

#endif
