#define _GNU_SOURCE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "imgmerge.h"
#include "mkdir_p.h"

#define DIR_MODE S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH

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

	if (minx % 2 == 1) minx--;
	if (miny % 2 == 1) miny--;
	if (maxx % 2 == 1) maxx++;
	if (maxy % 2 == 1) maxy++;

	minx /= 2;
	miny /= 2;
	maxx /= 2;
	maxy /= 2;

	for (int x = minx; x < maxx; x++) {
		for (int y = miny; y < maxy; y++) {
			int bx = x * 2;
			int by = y * 2;
			char *a, *b, *c, *d, *output, *blank, *dir;
			asprintf(&a, "./%s/images/0/%s/%d/%d.png", path, from, bx + 0, by + 0);
			asprintf(&b, "./%s/images/0/%s/%d/%d.png", path, from, bx + 1, by + 0);
			asprintf(&c, "./%s/images/0/%s/%d/%d.png", path, from, bx + 0, by + 1);
			asprintf(&d, "./%s/images/0/%s/%d/%d.png", path, from, bx + 1, by + 1);
			asprintf(&output, "./%s/images/0/%s/%d/%d.png", path, to, x, y);
			asprintf(&blank, "./%s/images/blank.png", path);
			asprintf(&dir, "./%s/images/0/%s/%d", path, to, x);
			mkdir_p(dir, DIR_MODE);
			int bc = 0;
			if (!file_exists(a)) { a = blank; bc++; }
			if (!file_exists(b)) { b = blank; bc++; }
			if (!file_exists(c)) { c = blank; bc++; }
			if (!file_exists(d)) { d = blank; bc++; }
			if (bc == 4) continue; // don't merge 4 blanks

			char* filenames[4] = {a, b, c, d};
			process_image(filenames, output);
		}
	}
}

