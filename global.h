#ifndef H_GLOBAL_H
#define H_GLOBAL_H

#include <ncurses.h>

#define FOOD_COLOR		0
#define BG_COLOR  		1
#define BORDER_COLOR 	2
#define BAR_COLOR		3
#define SNAKE_COLOR		4

#define WIDTH			80
#define HEIGHT			20
#define IN_COLOR(function, color) attron(COLOR_PAIR(color)); function; attroff(COLOR_PAIR(color))

#endif
