#include "snake.h"
#include "display.h"


int main() {
	display_init();	
	snake_start();
	endwin();

	return 0;
}
