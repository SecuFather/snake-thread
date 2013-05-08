#ifndef H_BOARD_H
#define H_BOARD_H

#include "global.h"

typedef struct {
	int field[HEIGHT][WIDTH];
} Board;

//inicjuje plansze
void board_init(Board *b);

//rysuje plansze
void board_draw(Board *b);

#endif
