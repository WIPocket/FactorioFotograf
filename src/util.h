#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

bool is_dir(char* path);
void xsymlink(char* a, char* b);
void xremove(char* path); // Try to remove but die if it fails
void write_file(char* path, const char* content, size_t lenght);

