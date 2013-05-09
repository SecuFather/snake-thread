#ifndef H_GLOBAL_H
#define H_GLOBAL_H

#include <pthread.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

//kolory poszczególnych elementów
#define FOOD_COLOR		1
#define BG_COLOR  		2
#define BORDER_COLOR 	3
#define BAR_COLOR		4
#define SNAKE_COLOR		5
#define SNAKE_COLOR2	6

//szerokość ekranu
#define WIDTH			80
//wysokość ekranu
#define HEIGHT			24

//makro pozwalające rysować w kolorze w danej funkcji
#define IN_COLOR(function, color) attron(COLOR_PAIR(color)); function; attroff(COLOR_PAIR(color))

//wylicza zawsze dodatnie modulo m
int pos_mod(int x, int m);

//zwraca 1 z prawdopodobieństwem 1/p
int rand_shot(int p);

//zamienia miejscami
void swap(int *x, int *y);

#endif
