#include <ucw/lib.h>
#include <ucw/gary.h>
#include <ucw-json/json.h>

#include <fcntl.h>

#define BUFFER_SIZE (1 << 13)

static struct json_context* js_;
static struct json_node* root_node_;

void mapinfo_read(char* path) {
	msg(L_DEBUG, "Reading mapinfo at '%s'", path);
	struct fastbuf* fb = bopen(path, O_RDONLY, BUFFER_SIZE);
	js_ = json_new();
	root_node_ = json_parse(js_, fb);
	bclose(fb);
}

void mapinfo(char* path, int* maxx, int* maxy, int* minx, int* miny) {
	mapinfo_read(path);

	*maxx = json_object_get(root_node_, "maxx")->number;
	*maxy = json_object_get(root_node_, "maxy")->number;
	*minx = json_object_get(root_node_, "minx")->number;
	*miny = json_object_get(root_node_, "miny")->number;

	json_delete(js_);
}
