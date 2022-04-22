#pragma once

#include <stdio.h>

#define log(fmt, ...) printf("[%.2f] - " fmt "\n", elapsed_sec(), ## __VA_ARGS__)
#define err(fmt, ...) printf("[%.2f] error - " fmt "\n", elapsed_sec(), ## __VA_ARGS__),exit(1)

float elapsed_sec(void);

