#ifndef H_DISPLAY_H
#define H_DISPLAY_H

#include "global.h"

//inicjowanie kolorów dla poszczególnych elementów
void display_color_init();

//inicjowanie wyświetlania
int display_init();

//inicjuje wątek pobierania danych z klawiatury
int display_init_key_thread(pthread_t *kt, char *c);

//pobiera w tle pobrane znaki z klawiatury
void *display_getch(void *c);

//kończy wyświetlanie
void display_finalize();

#endif
