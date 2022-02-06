#include <ucw/lib.h>
#include <ucw/opt.h>

#include <stdbool.h>
#include <time.h>
#include <sys/stat.h>

#include "util.h"
#include "modlist.h"
#include "factorio.h"

#include "fotograf/control.lua.asset.h"
#include "fotograf/info.json.asset.h"
#include "web/index.html.asset.h"
#include "web/script.js.asset.h"
#include "web/leaflet.permalink.min.js.asset.h"

static int png;
static char* save_name  = NULL;
static char* output_dir = "ff_output";
static char* fac_base   = NULL; // defaults to "%s/.factorio", where %s is expanded to env var $HOME
static char* fac_bin    = "/usr/bin/factorio";

static struct opt_section options = {
	OPT_ITEMS {
		OPT_HELP("Simple and speedy Factorio Map Generator."),
		OPT_HELP(""),
		OPT_HELP("Options:"),
		OPT_HELP_OPTION,

		OPT_BOOL  ('p', "png"      , png       , 0                 , "\tUse PNGs instead of JPGs"),
		OPT_STRING('s', "save-name", save_name , OPT_REQUIRED_VALUE, "\tSpecify save name"),
		OPT_STRING('o', "output"   , output_dir, OPT_REQUIRED_VALUE, "\tSpecify output folder"),
		OPT_STRING('b', "fac-base" , fac_base  , OPT_REQUIRED_VALUE, "\tOverride .factorio directory path"),
		OPT_STRING('e', "fac-bin"  , fac_bin   , OPT_REQUIRED_VALUE, "\tOverride factorio executable location"),

		OPT_END
	}
};

void parse_args(char* argv[]) {
	opt_parse(&options, argv+1);

	if (fac_base == NULL)
		fac_base = xasprintf("%s/.factorio", getenv("HOME"));

	msg(L_DEBUG, "argparse: png       : %d", png       );
	msg(L_DEBUG, "argparse: save_name : %s", save_name );
	msg(L_DEBUG, "argparse: output_dir: %s", output_dir);
	msg(L_DEBUG, "argparse: fac_base  : %s", fac_base  );
	msg(L_DEBUG, "argparse: fac_bin   : %s", fac_bin   );

	if (is_dir(output_dir))
		die("The output directory '%s' already exists.", output_dir);

	if (save_name == NULL)
		msg(L_INFO, "No save name provided. You will have to load the save manually.");
}

int main(int argc UNUSED, char* argv[]) {
	msg(L_INFO, "Hello.");

	parse_args(argv);

	char* modlist_json  = xasprintf("%s/mods/mod-list.json",             fac_base);
	char* image_dir     = xasprintf("%s/script-output/FF/images",        fac_base);
	char* map_info_json = xasprintf("%s/script-output/FF/map-info.json", fac_base);
	char* done_file     = xasprintf("%s/script-output/FF/done",          fac_base);

	if (is_dir(image_dir))
		die("The image output directory '%s' already exists.", image_dir);

	remove(done_file);     // these can silently fail if the file doesn't exist
	remove(map_info_json); // which is ok because we would remove them anyway

	{ // Paste fotograf into the mod directory
		char* fotograf_mod_dir = xasprintf("%s/mods/fotograf_1.0.0", fac_base);
		char* control_lua_file = xasprintf("%s/control.lua", fotograf_mod_dir);
		char* info_json_file   = xasprintf("%s/info.json"  , fotograf_mod_dir);
		remove(fotograf_mod_dir);
		mkdir(fotograf_mod_dir, S_IRWXU);
		write_file(control_lua_file, control_lua_asset, sizeof(control_lua_asset));
		write_file(info_json_file  , info_json_asset  , sizeof(info_json_asset)  );
		// TODO: set png flag
	}

	{ // write web files
		char* index_html_file = xasprintf("%s/index.html", output_dir);
		char* script_js_file  = xasprintf("%s/script.js",  output_dir);
		char* leaflet_js_file = xasprintf("%s/leaflet.permalink.min.js", output_dir);
		mkdir(output_dir, S_IRWXU);
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

	{ // Copy images
		// TODO
	}

	{ // Create blank image
		// TODO
	}

	{ // Zoom
		// TODO
	}

}

