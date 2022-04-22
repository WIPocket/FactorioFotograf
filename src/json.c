#include <ucw/lib.h>
#include <ucw/gary.h>
#include <ucw-json/json.h>

#include <fcntl.h>

#include "log.h"

static struct json_context* js;
static struct json_node* root_node;

static void jsonread(const char* filepath) {
	log("Reading json '%s'", filepath);
	struct fastbuf* fb = bopen(filepath, O_RDONLY, 256);
	js = json_new();
	root_node = json_parse(js, fb);
	bclose(fb);
}

static void jsonwrite(const char* filepath) {
	log("Writting json '%s'", filepath);
	struct fastbuf* fb = bopen(filepath, O_WRONLY | O_CREAT | O_TRUNC, 256);
	json_write(js, fb, root_node);
	bclose(fb);
	json_delete(js);
}

static void modlist_set(bool new_value) {
	log("%sabling fotograf in the modlist.", new_value ? "En" : "Dis");
	struct json_node* entry_array = json_object_get(root_node, "mods");

	// Iterating over all entries in the file
	for (size_t i = 0; i < GARY_SIZE(entry_array->elements); i++) {
		struct json_node* entry = entry_array->elements[i];
		const char* entry_name = json_object_get(entry, "name")->string;

		if (strcmp(entry_name, "fotograf") == 0) {
			struct json_node* new_json_node = json_new_bool(js, new_value);
			json_object_set(entry, "enabled", new_json_node);
			log("Found existing fotograf entry");
			return;
		}
	}

	log("fotograf entry in modlist not found, creating new one");

	struct json_node* new_entry = json_new_object(js);
	json_object_set(new_entry, "name", json_new_string(js, "fotograf"));
	json_object_set(new_entry, "enabled", json_new_bool(js, new_value));

	json_array_append(entry_array, new_entry);
}

void modlist(const char* path, bool val) {
	jsonread(path);
	modlist_set(val);
	jsonwrite(path);
}

void mapinfo(char* path, int* maxx, int* maxy, int* minx, int* miny) {
	jsonread(path);

	*maxx = json_object_get(root_node, "maxx")->number;
	*maxy = json_object_get(root_node, "maxy")->number;
	*minx = json_object_get(root_node, "minx")->number;
	*miny = json_object_get(root_node, "miny")->number;

	json_delete(js);
}

