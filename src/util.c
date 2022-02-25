#include <ucw/lib.h>

#include <stdio.h>

#include "util.h"

bool is_dir(char* path) {
	DIR* dir = opendir(path);
	if (dir) {
		closedir(dir);
		return true;
	}
	return false;
}

void write_file(char* path, const char* content, size_t lenght) {
	msg(L_DEBUG, "Writing %zu bytes to '%s'", lenght, path);
	FILE* f = fopen(path, "w");
	if (f == 0)
		die("failed to open file '%s'.", path);
	fwrite(content, lenght, 1, f);
	fclose(f);
}

