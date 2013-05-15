#ifndef H_BOARD_H
#define H_BOARD_H

#include "global.h"
#include "display.h"

typedef struct {
	int field[HEIGHT][WIDTH];
	int score[2];
	int best_score[2];
} Board;

pthread_mutex_t field_mutex[HEIGHT][WIDTH];
pthread_mutex_t board_mutex;
pthread_mutex_t score_mutex;
pthread_mutex_t pause_mutex;

//inicjuje plansze
void board_init(Board *b);

//rysuje plansze
void board_draw(Board *b);

//losuje pozycję z tłem, i blokuje to pole
void board_rand_bg(Board *b, int *x, int *y);

//pokazuje wynik
void board_show_score(Board *b);

//inicjuje mutexy
void board_init_mutex();

//kończy mutexy
void board_finalize_mutex();

//ustawia wartość dla pola o współrzędnych x, y i odblokowuje przyporządkowany mu mutex
void board_set_field(Board *b, int x, int y, int v);

#endif
