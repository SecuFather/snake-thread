#ifndef H_BOARD_H
#define H_BOARD_H

#include "global.h"

typedef struct {
	int field[HEIGHT][WIDTH];
} Board;

//inicjuje plansze
void init_board(Board *b);

//rysuje plansze
void draw_board(Board *b);

#endif
