#ifndef H_DISPLAY_H
#define H_DISPLAY_H

#include "global.h"

//inicjowanie kolorów dla poszczególnych elementów
void display_color_init();

//inicjowanie wyświetlania
int display_init();

//pobiera w tle pobrane znaki z klawiatury
void *display_getch(void *c);

//kończy wyświetlanie
void display_finalize();

#endif
