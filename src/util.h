#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

bool is_dir(char* path);
void write_file(char* path, const char* content, size_t lenght);

