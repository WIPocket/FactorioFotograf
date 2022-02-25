#include <ucw/lib.h>
#include <ucw/opt.h>
#include <ucw/workqueue.h>
#include <ucw/stkstring.h>

#include <sys/stat.h>
#include <dirent.h>

#include "modlist.h"
#include "factorio.h"
#include "img.h"
#include "mapinfo.h"

#include "fotograf/control.lua.asset.h"
#include "fotograf/info.json.asset.h"
#include "web/index.html.asset.h"
#include "web/script.js.asset.h"
#include "web/leaflet.permalink.min.js.asset.h"

int png, alt_mode, jobs = 4, jpg_quality = 90, dist = 128, ppt = 32;
double daytime = 1.0;
static char *fac_base, *save_name, *fac_bin = "/usr/bin/factorio";

static bool is_dir(char* path) {
	DIR* dir = opendir(path);
	if (dir) {
		closedir(dir);
		return true;
	}
	return false;
}

static void write_file(char* path, const char* content, size_t lenght) {
	msg(L_DEBUG, "Writing %zu bytes to '%s'", lenght, path);
	FILE* f = fopen(path, "w");
	if (f == 0)
		die("failed to open file '%s'.", path);
	fwrite(content, lenght, 1, f);
	fclose(f);
}

static struct opt_section options = {
	OPT_ITEMS {
		OPT_HELP("Simple and speedy Factorio Map Generator."),
		OPT_HELP(""),
		OPT_HELP("Options:"),
		OPT_HELP_OPTION,
		OPT_BOOL  ('p', "png"        , png        , 0                 , "\tUse PNGs instead of JPGs (default false)"),
		OPT_STRING('s', "save-name"  , save_name  , OPT_REQUIRED_VALUE, "\tSpecify save name"),
		OPT_STRING('b', "fac-base"   , fac_base   , OPT_REQUIRED_VALUE, "\tOverride .factorio directory path (default $HOME/.factorio)"),
		OPT_STRING('e', "fac-bin"    , fac_bin    , OPT_REQUIRED_VALUE, "\tOverride Factorio executable location (default /usr/bin/factorio)"),
		OPT_DOUBLE('t', "daytime"    , daytime    , OPT_REQUIRED_VALUE, "\tFactorio daytime (default 1.0)"),
		OPT_BOOL  ('a', "alt-mode"   , alt_mode   , 0                 , "\tEnable alt-mode for screenshots (default false)"),
		OPT_INT   ('j', "jobs"       , jobs       , OPT_REQUIRED_VALUE, "\tThe number of threads to create (default 4)"),
		OPT_INT   ('q', "jpg-quality", jpg_quality, OPT_REQUIRED_VALUE, "\tJpg quality to use when creating jpgs (0-100) (default 90)"),
		OPT_INT   ('d', "dist"       , dist       , OPT_REQUIRED_VALUE, "\tMaximum distance from chunk to player structure (default 128)"),
		OPT_INT   (0  , "ppt"        , ppt        , OPT_REQUIRED_VALUE, "\tPixels per Factorio-tile (default 32)"),
		OPT_END
	}
};

static void parse_args(char* argv[]) {
	opt_parse(&options, argv+1);

	if (fac_base == NULL)
		asprintf(&fac_base, "%s/.factorio", getenv("HOME"));

	if (save_name == NULL)
		msg(L_INFO, "No save name provided. You will have to load the save manually.");
}

int main(int argc UNUSED, char* argv[]) {
	msg(L_INFO, "Hello.");

	parse_args(argv);

	char* modlist_json = stk_printf("%s/mods/mod-list.json", fac_base);
	char* ff_dir       = stk_printf("%s/script-output/FF",   fac_base);

	char* done_file = stk_printf("%s/done", ff_dir);

	if (is_dir(ff_dir))
		die("The directory '%s' already exists.", ff_dir);

	{ // Paste fotograf into the mod directory
		char* fotograf_mod_dir = stk_printf("%s/mods/fotograf_1.0.0", fac_base);
		char* control_lua_file = stk_printf("%s/control.lua", fotograf_mod_dir);
		char* info_json_file   = stk_printf("%s/info.json"  , fotograf_mod_dir);
		mkdir(fotograf_mod_dir, S_IRWXU);
		write_file(info_json_file, info_json_asset, sizeof(info_json_asset));

		FILE* fd = fopen(control_lua_file, "w");
		fprintf(fd, control_lua_asset, (png ? "png" : "jpg"), ppt, dist, jpg_quality, daytime, (alt_mode ? "true" : "false"));
		fclose(fd);
	}

	{ // Paste web files
		char* index_html_file = stk_printf("%s/index.html", ff_dir);
		char* script_js_file  = stk_printf("%s/script.js",  ff_dir);
		char* leaflet_js_file = stk_printf("%s/leaflet.permalink.min.js", ff_dir);
		mkdir(ff_dir, S_IRWXU);
		write_file(index_html_file, index_html_asset, sizeof(index_html_asset)-1);
		write_file(script_js_file,  script_js_asset,  sizeof(script_js_asset)-1);
		write_file(leaflet_js_file, leaflet_permalink_min_js_asset, sizeof(leaflet_permalink_min_js_asset)-1);
	}

	{ // Run factorio and capture images
		// Enable fotograf mod in the modlist json file
		modlist(modlist_json, true);

		run_factorio(fac_bin, done_file, save_name);

		// Disable fotograf mod in the modlist json file
		modlist(modlist_json, false);
	}

	int maxx, maxy, minx, miny;
	{ // Read map_info.json and create map_info.js
		char* map_info_json = stk_printf("%s/map_info.json", ff_dir);
		char* map_info_js   = stk_printf("%s/map_info.js"  , ff_dir);
		mapinfo(map_info_json, &maxx, &maxy, &minx, &miny);
		printf("%d %d %d %d\n", maxx, maxy, minx, miny);

		// copy the json content into a js file with string variable of this content
		FILE* json = fopen(map_info_json, "r");
		fseek(json, 0, SEEK_END);
		size_t length = ftell(json);
		fseek(json, 0, SEEK_SET);

		char* buffer = alloca(length+1);
		fread(buffer, length, 1, json);
		fclose(json);
		buffer[length] = 0;

		FILE* js = fopen(map_info_js, "w");
		fprintf(js, "map_info_string = '%s'", buffer);
		fclose(js);
	}

	{ // Create blank image and zoomout
		char* blank_file = stk_printf("%s/images/blank.%s", ff_dir, (png ? "png" : "jpg"));
		create_blank(blank_file, ppt*32, false);

		struct worker_pool pool = {
			.num_threads = jobs,
			.stack_size = 65536,
		};
		worker_pool_init(&pool);

		struct work_queue q;
		work_queue_init(&pool, &q);

		for (int i = 8; i > 0; i--) {
			zoomout(&q, ff_dir, blank_file, i, &maxx, &maxy, &minx, &miny);
			msg(L_INFO, "Finished zoom level (%d/8)", 9-i);
		}

		work_queue_cleanup(&q);
		worker_pool_cleanup(&pool);
	}

	msg(L_INFO, "Done! Map is in '%s'", ff_dir);
}

