#include "snake.h"


int main() {
	if (display_init()) {
		return 1;
	}
	snake_start();
	
	display_finalize();
	printf("Thank you for watching!\n");
	pthread_exit(NULL);
	return 0;
}
