#ifndef H_SNAKE_H
#define H_SNAKE_H

#include "global.h"
#include "food.h"


#define SNAKE_SIZE			100
#define SNAKE_COUNT			20
#define SNAKE_START_SIZE	4
#define SNAKE_DELAY			20000

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
	int color;
	int team;
	int grow;
} Snake;

Snake s[SNAKE_COUNT];
Food f;
Board b;
int snake_finish, snake_pause, snake_step;
pthread_mutex_t snake_mutex[SNAKE_COUNT];
pthread_mutex_t step_mutex;
pthread_cond_t step_cond;

//inicjuje węża
void snake_init(Snake *s, Board *b, int id);

//sprawdza czy nastąpiły warunki do zakończenia programu
int snake_crash(Snake *s, char *c, pthread_t *kt);

//sprawdza czy wąż natrafił na pożywienie, jeśli tak, rośnie
void snake_eat_and_grow(Snake *s, Board *b, Food *f);

//porusza wężem
void snake_move(Snake *s, Board *b, Food *f);

//szuka głowy kolegi z drużyny
int snake_check_head(Snake *s, Board *b);

//sprawdza czy natrafiono na ogon przeciwnika
int snake_check_tail(Snake *s, Board *b, int x, int y);

//wylicza opłacalność dla określonego ruchu
int snake_rate_direction(Snake *s, Board *b, Food *f, int x, int y, int fd, pthread_mutex_t **m, int *best, int *best_result, int id);

//wylicza opłacalność danego ruchu od 0 - porażka do 4 - idealny
int snake_check_direction(Snake *s, Board *b, Food *f, char dir, pthread_mutex_t **m, int *best, int *best_result, int id);

//ucina ogon
void snake_cut_tail(Snake *s, Board *b);

//wylicza kierunek podążania węża, zwraca zero gdy wąż umiera
int snake_decide(Snake *s, Board *b, Food *f);

//zmienia rozmiar węża, zwraca 1 jeśli zmieniono rozmiar
int snake_change_size(Snake *s, Board *b, int x);

//niszczy węża
void snake_destroy(Snake *s, Board *b);

//obraca węża
void snake_reverse(Snake *s);

//wątek węża
void *snake_thread(void *id);

//startuje węża
void snake_start();

//inicjuje mutexy
void snake_init_mutex();

//niszczy mutexy
void snake_finalize_mutex();



#endif
