#include <ucw/lib.h>
#include <ucw/opt.h>

#include <stdbool.h>
#include <time.h>
#include <sys/stat.h>
#include <alloca.h>

#include "util.h"
#include "modlist.h"
#include "factorio.h"
#include "img.h"
#include "mapinfo.h"

#include "fotograf/control.lua.asset.h"
#include "fotograf/info.json.asset.h"
#include "web/index.html.asset.h"
#include "web/script.js.asset.h"
#include "web/leaflet.permalink.min.js.asset.h"

static int png, min_dist = 128, ppt = 32;
static char* save_name  = NULL;
static char* fac_base   = NULL; // defaults to "%s/.factorio", where %s is expanded to env var $HOME
static char* fac_bin    = "/usr/bin/factorio";

#define EXT (png ? "png" : "jpg")

static struct opt_section options = {
	OPT_ITEMS {
		OPT_HELP("Simple and speedy Factorio Map Generator."),
		OPT_HELP(""),
		OPT_HELP("Options:"),
		OPT_HELP_OPTION,

		OPT_BOOL  ('p', "png"      , png       , 0                 , "\tUse PNGs instead of JPGs"),
		OPT_STRING('s', "save-name", save_name , OPT_REQUIRED_VALUE, "\tSpecify save name"),
		OPT_STRING('b', "fac-base" , fac_base  , OPT_REQUIRED_VALUE, "\tOverride .factorio directory path"),
		OPT_STRING('e', "fac-bin"  , fac_bin   , OPT_REQUIRED_VALUE, "\tOverride factorio executable location"),
		OPT_INT   ('d', "min-dist" , min_dist  , OPT_REQUIRED_VALUE, "\tMinimum distance from chunk center to any structure to include this chunk in the map"),
		OPT_INT   (0  , "ppt"      , ppt       , OPT_REQUIRED_VALUE, "\tPixels per factorio-tile"),

		OPT_END
	}
};

void parse_args(char* argv[]) {
	opt_parse(&options, argv+1);

	if (fac_base == NULL)
		fac_base = xasprintf("%s/.factorio", getenv("HOME"));

	msg(L_DEBUG, "argparse: png      : %d", png       );
	msg(L_DEBUG, "argparse: save_name: %s", save_name );
	msg(L_DEBUG, "argparse: fac_base : %s", fac_base  );
	msg(L_DEBUG, "argparse: fac_bin  : %s", fac_bin   );

	if (save_name == NULL)
		msg(L_INFO, "No save name provided. You will have to load the save manually.");
}

int main(int argc UNUSED, char* argv[]) {
	msg(L_INFO, "Hello.");

	parse_args(argv);

	char* modlist_json = xasprintf("%s/mods/mod-list.json", fac_base);
	char* ff_dir       = xasprintf("%s/script-output/FF",   fac_base);

	char* done_file = xasprintf("%s/done", ff_dir);

	if (is_dir(ff_dir))
		die("The directory '%s' already exists.", ff_dir);

	{ // Paste fotograf into the mod directory
		char* fotograf_mod_dir = xasprintf("%s/mods/fotograf_1.0.0", fac_base);
		char* control_lua_file = xasprintf("%s/control.lua", fotograf_mod_dir);
		char* info_json_file   = xasprintf("%s/info.json"  , fotograf_mod_dir);
		mkdir(fotograf_mod_dir, S_IRWXU);
		write_file(info_json_file, info_json_asset, sizeof(info_json_asset));

		FILE* fd = fopen(control_lua_file, "w");
		fprintf(fd, control_lua_asset, EXT, ppt, min_dist);
		fclose(fd);
	}

	{ // Paste web files
		char* index_html_file = xasprintf("%s/index.html", ff_dir);
		char* script_js_file  = xasprintf("%s/script.js",  ff_dir);
		char* leaflet_js_file = xasprintf("%s/leaflet.permalink.min.js", ff_dir);
		mkdir(ff_dir, S_IRWXU);
		write_file(index_html_file, index_html_asset, sizeof(index_html_asset));
		write_file(script_js_file,  script_js_asset,  sizeof(script_js_asset));
		write_file(leaflet_js_file, leaflet_permalink_min_js_asset, sizeof(leaflet_permalink_min_js_asset));
	}

	{ // Run factorio and capture images
		// Enable fotograf mod in the modlist json file
		modlist(modlist_json, true);

		run_factorio(fac_bin, done_file);

		// Disable fotograf mod in the modlist json file
		modlist(modlist_json, false);
	}

	{ // Read map_info.json and create map_info.js
		char* map_info_json = xasprintf("%s/map_info.json", ff_dir);
		char* map_info_js   = xasprintf("%s/map_info.js"  , ff_dir);
		int maxx, maxy, minx, miny;
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

		FILE* js   = fopen(map_info_js,   "w");
		fprintf(js, "map_info_string = '%s'", buffer);
		fclose(js);
	}

	{ // Create blank image
		char* blank_file = xasprintf("%s/images/blank.%s", ff_dir, EXT);
		create_blank(blank_file, ppt*32, false);
	}

	{ // Zoom
		// TODO
	}

}

