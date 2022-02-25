#pragma once
#include <ucw/lib.h>
#include <ucw/workqueue.h>

void create_blank(char* path, int s, bool png);
void zoomout(struct work_queue* q, char* path, char* blank, int from, int maxx, int maxy, int minx, int miny);

