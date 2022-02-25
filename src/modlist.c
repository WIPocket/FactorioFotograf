#include <ucw/lib.h>
#include <ucw/gary.h>
#include <ucw-json/json.h>

#include <fcntl.h>

#define BUFFER_SIZE (1 << 13)

static struct json_context* js;
static struct json_node* root_node;

void modlist_read(const char* filepath) {
	msg(L_DEBUG, "Reading modlist at '%s'", filepath);
	struct fastbuf* fb = bopen(filepath, O_RDONLY, BUFFER_SIZE);
	js = json_new();
	root_node = json_parse(js, fb);
	bclose(fb);
}

void modlist_write(const char* filepath) {
	msg(L_DEBUG, "Writting modlist");
	struct fastbuf* fb = bopen(filepath, O_WRONLY | O_CREAT | O_TRUNC, BUFFER_SIZE);
	json_write(js, fb, root_node);
	bclose(fb);
	json_delete(js);
}

void modlist_set(bool new_value) {
	msg(L_DEBUG, "%sabling fotograf in the modlist.", new_value ? "En" : "Dis");
	struct json_node* entry_array = json_object_get(root_node, "mods");

	// Iterating over all entries in the file
	for (size_t i = 0; i < GARY_SIZE(entry_array->elements); i++) {
		struct json_node* entry = entry_array->elements[i];
		const char* entry_name = json_object_get(entry, "name")->string;

		if (strcmp(entry_name, "fotograf") == 0) {
			struct json_node* new_json_node = json_new_bool(js, new_value);
			json_object_set(entry, "enabled", new_json_node);
			msg(L_DEBUG, "Found existing fotograf entry");
			return;
		}
	}

	msg(L_DEBUG, "fotograf entry in modlist not found, creating new one");

	struct json_node* new_entry = json_new_object(js);
	json_object_set(new_entry, "name", json_new_string(js, "fotograf"));
	json_object_set(new_entry, "enabled", json_new_bool(js, new_value));

	json_array_append(entry_array, new_entry);
}

void modlist(const char* path, bool val) {
	modlist_read(path);
	modlist_set(val);
	modlist_write(path);
}

