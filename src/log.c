#include <ucw/lib.h>
#include <sys/time.h>

static struct timeval start;

static void CONSTRUCTOR init_timer(void) {
	gettimeofday(&start, NULL);
}

float elapsed_sec(void) {
	struct timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec  - start.tv_sec) + (now.tv_usec - start.tv_usec) * 0.000001;
}

