#ifndef H_SNAKE_H
#define H_SNAKE_H

#include "global.h"
#include "food.h"
#include "display.h"

#define SNAKE_SIZE			100
#define SNAKE_COUNT			10
#define SNAKE_START_SIZE	4
#define SNAKE_DELAY			100000
#define SYS_DELAY			1000

#define NORTH		0
#define EAST		1
#define SOUTH		2
#define WEST		3

#define RIGHT		1
#define LEFT		-1

#define ALIVE		1
#define DEAD		0

typedef struct {
	int id;
	char label[2];
	int x[SNAKE_SIZE];
	int y[SNAKE_SIZE];
	int steps;
	int isize;
	int size;
	int turn;
	int alive;
	char dir;
} Snake;

Snake s[SNAKE_COUNT];
Food f;
Board b;
int current;

//inicjuje węża
void snake_init(Snake *s, Board *b, int id);

//sprawdza czy nastąpiły warunki do zakończenia programu
int snake_crash(Snake *s, char c, pthread_t *kt);

//sprawdza czy wąż natrafił na pożywienie, jeśli tak, rośnie
void snake_eat_and_grow(Snake *s, Board *b, Food *f);

//porusza wężem
void snake_move(Snake *s, Board *b, Food *f);

//wylicza opłacalność danego ruchu od 0 - porażka do 2 - idealny
int snake_check_direction(Snake *s, Board *b, Food *f, char dir);

//wylicza kierunek podążania węża, zwraca zero gdy wąż umiera
int snake_decide(Snake *s, Board *b, Food *f);

//niszczy węża
void snake_destroy(Snake *s, Board *b);

//obraca węża
void snake_reverse(Snake *s);

//wątek węża
void *snake_thread(void *id);

//startuje węża
void snake_start();


#endif
