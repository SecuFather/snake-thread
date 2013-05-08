#include "global.h"

int pos_mod(int x, int m) {
	return (x+m)%m;
}

int rand_shot(int p) {
	return rand()%p == 0;
}
