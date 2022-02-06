#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

// Try to do asprintf but die if it fails.
#define xasprintf(fmt, ...) \
({ \
	char* _str; \
	int _res = asprintf(&_str, fmt, __VA_ARGS__); \
	if (_res == -1) { \
		die("failed to asprintf."); \
	} \
	_str;\
})

bool is_dir(char* path);
void xremove(char* path); // Try to remove but die if it fails
void write_file(char* path, const char* content, size_t lenght);

