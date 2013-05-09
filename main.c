#include "snake.h"


int main() {
	display_add_log("inicjowanie wyświetlacza");
	if (display_init()) {
		return -1;
	}

	snake_start();
	
	display_add_log("zwalnianie zasobów...");
	display_finalize();

	display_add_log("koniec programu");
	pthread_exit(NULL);
}
