#ifndef H_BOARD_H
#define H_BOARD_H

#include "global.h"

typedef struct {
	int field[HEIGHT][WIDTH];
	int score[2];
	int best_score[2];
} Board;

//inicjuje plansze
void board_init(Board *b);

//rysuje plansze
void board_draw(Board *b);

//losuje pozycję z tłem
void board_rand_bg(Board *b, int *x, int *y);

//pokazuje wynik
void board_show_score(Board *b);

#endif
