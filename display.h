#ifndef H_DISPLAY_H
#define H_DISPLAY_H

#include "global.h"

pthread_mutex_t crash_mutex;
pthread_cond_t crash_cond;

//inicjowanie kolorów dla poszczególnych elementów
void display_color_init();

//inicjowanie wyświetlania
int display_init();

//pobiera w tle pobrane znaki z klawiatury
void *display_getch(void *c);

//kończy wyświetlanie
void display_finalize();

//inicjuje loga
void display_init_log();

//dodaje do loga
void display_add_log(const char *str);

#endif
