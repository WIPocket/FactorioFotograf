#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "thpool.h"
#include "imgmerge.h"

#define PATH_LEN 256

bool file_exists(char* filename) { return access(filename, R_OK | W_OK) == 0; }

int main(int argc, char* argv[]) {
	if (argc != 8) {
		printf("wrong usage\n");
		exit(1);
	}

	char* path =    argv[1];
	char* from =    argv[2];
	char* to   =    argv[3];
	int maxx = atoi(argv[4]);
	int maxy = atoi(argv[5]);
	int minx = atoi(argv[6]);
	int miny = atoi(argv[7]);

	threadpool pool = thpool_init(sysconf(_SC_NPROCESSORS_ONLN));

	if (minx % 2 == 1) minx--;
	if (miny % 2 == 1) miny--;
	if (maxx % 2 == 1) maxx++;
	if (maxy % 2 == 1) maxy++;

	minx /= 2;
	miny /= 2;
	maxx /= 2;
	maxy /= 2;

	char *blank = calloc(1, PATH_LEN * sizeof(char*));
	sprintf(blank, "./%s/images/blank.png", path);

	for (int x = minx; x < maxx; x++) {
		for (int y = miny; y < maxy; y++) {
			int bx = x * 2;
			int by = y * 2;

			char *a = calloc(PATH_LEN, sizeof(char*));
			char *b = calloc(PATH_LEN, sizeof(char*));
			char *c = calloc(PATH_LEN, sizeof(char*));
			char *d = calloc(PATH_LEN, sizeof(char*));
			char *o = calloc(PATH_LEN, sizeof(char*));

			sprintf(a, "./%s/images/0/%s/%d_%d.png", path, from, bx + 0, by + 0);
			sprintf(b, "./%s/images/0/%s/%d_%d.png", path, from, bx + 1, by + 0);
			sprintf(c, "./%s/images/0/%s/%d_%d.png", path, from, bx + 0, by + 1);
			sprintf(d, "./%s/images/0/%s/%d_%d.png", path, from, bx + 1, by + 1);
			sprintf(o, "./%s/images/0/%s/%d_%d.png", path, to  , x     , y     );

			int bc = 0;
			if (!file_exists(a)) { free(a); a = blank; bc++; }
			if (!file_exists(b)) { free(b); b = blank; bc++; }
			if (!file_exists(c)) { free(c); c = blank; bc++; }
			if (!file_exists(d)) { free(d); d = blank; bc++; }
			if (bc == 4) {
				free(o);
				continue; // don't merge 4 blanks
			}

			char** filenames = malloc(5 * sizeof(char*));
			filenames[0] = a;
			filenames[1] = b;
			filenames[2] = c;
			filenames[3] = d;
			filenames[4] = o;

			thpool_add_work(pool, process_image, (void*)filenames);
		}
	}

	thpool_wait(pool);
	thpool_destroy(pool);

	free(blank);
}

