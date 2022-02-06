#include <ucw/lib.h>

#include <stdio.h>

#include "util.h"

#define CHECKED(expected, func, ...) \
do { \
	int _res = func(__VA_ARGS__); \
	if (_res != expected) \
		die(#func " failed (%d).", _res); \
} while (0)

bool is_dir(char* path) {
	DIR* dir = opendir(path);
	if (dir) {
		closedir(dir);
		return true;
	}
	return false;
}

void xremove(char* path) {
	msg(L_DEBUG, "Removing %s.", path);
	CHECKED(0, remove, path);
}

void xsymlink(char* a, char* b) {
	msg(L_DEBUG, "Symlinking %s to %s.", b, a);
	CHECKED(0, symlink, a, b);
}

void write_file(char* path, const char* content, size_t lenght) {
	msg(L_DEBUG, "Writing %zu bytes to '%s'", lenght, path);
	FILE* f = fopen(path, "w");
	if (f == 0)
		die("failed to open file '%s'.", path);
	fwrite(content, lenght, 1, f);
	fclose(f);
}

