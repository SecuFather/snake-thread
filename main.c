#include "snake.h"


int main() {
	if (display_init()) {
		return 1;
	}
	snake_start();
	
	display_finalize();
	return 0;
}
