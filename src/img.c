#include "img.h"

#include <ucw/lib.h>
#include <ucw/workqueue.h>
#include <ucw/stkstring.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "blank.png.asset.h"

#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG

#define PATH_LEN 256

#include "stb_image.h"
#include "stb_image_resize.h"
#include "stb_image_write.h"

extern bool png; // from main.c; if we are working with PNGs instead of JPEGs

// A work payload for the threads
struct w {
	struct work w;
	char* filenames[5];
	int x, y;
};

void create_blank(char* path, int s, bool png) {
	msg(L_DEBUG, "Writting blank %s image %dx%d to '%s'", png ? "png" : "jpg", s, s, path);
	int x, y, n;
	// sizeof-1 because biem automatically appends terminating zero at the end
	u8* blank = stbi_load_from_memory(blank_png_asset, sizeof(blank_png_asset)-1, &x, &y, &n, 3);

	u8* blank_resized = xmalloc(s*s*3);

	stbir_resize_uint8(blank, x, y, 0, blank_resized, s, s, 0, 3);

	stbi_image_free(blank);

	if (png)
		stbi_write_png(path, s, s, 3, blank_resized, 0);
	else /* jpg */
		stbi_write_jpg(path, s, s, 3, blank_resized, 0);

	xfree(blank_resized);
}

bool file_exists(char* filename) {
	return access(filename, R_OK | W_OK) == 0;
}

bool is_blank(char* str) {
	size_t l = strlen(str);
	return str[l-5] == 'k'; // check for the 'k' in 'blank.{png,jpg}'
}

void __attribute__((hot)) go(struct worker_thread *t UNUSED, struct work *wrk) {
	struct w* payload = (struct w*) wrk;

	int w, h, n;
	unsigned char *imgs[4];
	for (int i = 0; i < 4; i++) {
		unsigned char *big = stbi_load(payload->filenames[i], &w, &h, &n, 3);
		imgs[i] = xmalloc(w * h * 3);
		stbir_resize_uint8(big, w, h, 0, imgs[i], w / 2, h / 2, 0, 3);
		stbi_image_free(big);
	}
	w /= 2; // the images get scaled down
	h /= 2;

	// MERGE IMAGES

	int mw = w * 2; // final image is 2x2 smaller images
	int mh = h * 2;
	unsigned char* merged = xmalloc(mw * mh * 3); // 3 chars per pixel (rgb)

	for (int l = 0; l < h; l++) { memcpy(&merged[(((l    ) * mw)    ) * 3], &imgs[0][l * w * 3], w * 3); }
	for (int l = 0; l < h; l++) { memcpy(&merged[(((l    ) * mw) + w) * 3], &imgs[1][l * w * 3], w * 3); }
	for (int l = 0; l < h; l++) { memcpy(&merged[(((l + h) * mw)    ) * 3], &imgs[2][l * w * 3], w * 3); }
	for (int l = 0; l < h; l++) { memcpy(&merged[(((l + h) * mw) + w) * 3], &imgs[3][l * w * 3], w * 3); }

	for (int i = 0; i < 4; i++) free(imgs[i]);

	// WRITE MERGED IMAGE

	if (png)
		stbi_write_png(payload->filenames[4], mw, mh, 3, merged, 0);
	else
		stbi_write_jpg(payload->filenames[4], mw, mh, 3, merged, 90);

	xfree(merged);

	for (int i = 0; i < 4; i++)
		if (!is_blank(payload->filenames[i]))
			xfree(payload->filenames[i]);

	free(payload->filenames[4]);
}

void zoomout(struct work_queue* q, char* path, char* blank, int from, int maxx, int maxy, int minx, int miny) {
	// shring bounds TODO: is this just round up/down?
	if (minx % 2 == 1) minx -= 1;
	if (miny % 2 == 1) miny -= 1;
	if (maxx % 2 == 1) maxx += 1;
	if (maxy % 2 == 1) maxy += 1;

	minx /= 2;
	miny /= 2;
	maxx /= 2;
	maxy /= 2;

	char* ext = png ? "png" : "jpg";
	int submitted = 0;

	char* out_dir = stk_printf("%s/images/0/%d", path, from-1);
	mkdir(out_dir, 0700);

	for (int x = minx; x < maxx; x++) {
		for (int y = minx; y < maxy; y++) {
			int bx = x * 2;
			int by = y * 2;

			char *a = xmalloc_zero(PATH_LEN * sizeof(char*));
			char *b = xmalloc_zero(PATH_LEN * sizeof(char*));
			char *c = xmalloc_zero(PATH_LEN * sizeof(char*));
			char *d = xmalloc_zero(PATH_LEN * sizeof(char*));
			char *o = xmalloc_zero(PATH_LEN * sizeof(char*));

			sprintf(a, "%s/images/0/%d/%d_%d.%s", path, from  , bx + 0, by + 0, ext);
			sprintf(b, "%s/images/0/%d/%d_%d.%s", path, from  , bx + 1, by + 0, ext);
			sprintf(c, "%s/images/0/%d/%d_%d.%s", path, from  , bx + 0, by + 1, ext);
			sprintf(d, "%s/images/0/%d/%d_%d.%s", path, from  , bx + 1, by + 1, ext);
			sprintf(o, "%s/images/0/%d/%d_%d.%s", path, from-1, x     , y     , ext);

			int bc = 0;
			if (!file_exists(a)) { free(a); a = blank; bc++; }
			if (!file_exists(b)) { free(b); b = blank; bc++; }
			if (!file_exists(c)) { free(c); c = blank; bc++; }
			if (!file_exists(d)) { free(d); d = blank; bc++; }
			if (bc == 4) {
				free(o);
				continue; // don't merge 4 blanks
			}

			struct w *w = xmalloc_zero(sizeof(struct w));
			w->w.go = go;
			w->w.priority = 0;
			w->x = x;
			w->y = y;
			w->filenames[0] = a;
			w->filenames[1] = b;
			w->filenames[2] = c;
			w->filenames[3] = d;
			w->filenames[4] = o;

			work_submit(q, &w->w);
			msg(L_INFO, "Submitted job %d %d", w->x, w->y);
			submitted++;
		}
	}

	struct w *w;
	int finished = 0;
	while ((w = (struct w*) work_wait(q))) {
		msg(L_INFO, "Finished job %d %d (%d/%d)", w->x, w->y, finished, submitted);
		finished++;
		free(w);
	}
}

