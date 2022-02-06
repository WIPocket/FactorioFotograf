#include "img.h"

#include <ucw/lib.h>

#include "blank.png.asset.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void create_blank(char* path, int s, bool png) {
	msg(L_DEBUG, "Writting blank %s image %dx%d to '%s'", png ? "png" : "jpg", s, s, path);
	int x = 1, y = 1, n = 3;
	// sizeof-1 because biem automatically appends terminating zero at the end
	u8* blank = stbi_load_from_memory(blank_png_asset, sizeof(blank_png_asset)-1, &x, &y, &n, 3);

	u8* blank_resized = xmalloc(s*s*3);

	stbir_resize_uint8(
			blank, x, y, 0,
			blank_resized, s, s, 0, 3
	);

	stbi_image_free(blank);

	if (png)
		stbi_write_png(path, s, s, 3, blank_resized, 0);
	else /* jpg */
		stbi_write_jpg(path, s, s, 3, blank_resized, 0);

	xfree(blank_resized);
}
