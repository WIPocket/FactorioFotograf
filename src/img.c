#include "img.h"

#include <ucw/lib.h>
#include <ucw/workqueue.h>
#include <ucw/stkstring.h>

#include <sys/stat.h>
#include <unistd.h>

#include "log.h"
#include "blank.png.asset.h"

#define PATH_LEN 256

#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG

#include "stb_image.h"
#include "stb_image_resize.h"
#include "stb_image_write.h"

extern int png, jpg_quality; // arg options from main.c

// A work payload for the threads
struct w {
	struct work w;
	char paths[PATH_LEN*5];
	int x, y;
};

void create_blank(char* path, int s, bool png) {
	log("Writting blank %s image %dx%d to '%s'.", png ? "png" : "jpg", s, s, path);
	int x, y, n;
	// sizeof-1 because biem automatically appends terminating zero at the end
	byte* blank = stbi_load_from_memory(blank_png_asset, sizeof(blank_png_asset)-1, &x, &y, &n, 3);

	byte* blank_resized = xmalloc(s*s*3);

	stbir_resize_uint8(blank, x, y, 0, blank_resized, s, s, 0, 3);

	stbi_image_free(blank);

	if (png) stbi_write_png(path, s, s, 3, blank_resized, 0);
	else     stbi_write_jpg(path, s, s, 3, blank_resized, 80);

	xfree(blank_resized);
}

static void go(struct worker_thread *t UNUSED, struct work *wrk) {
	struct w* payload = (struct w*) wrk;

	int w, h, n;
	byte* imgs[4];
	for (int i = 0; i < 4; i++) {
		byte* big = stbi_load(payload->paths + PATH_LEN * i, &w, &h, &n, 3);

		if (!big)
			die("Failed to load file '%s'. '%s'", payload->paths + PATH_LEN * i, stbi_failure_reason());

		imgs[i] = xmalloc(w * h * 3);
		stbir_resize_uint8(big, w, h, 0, imgs[i], w / 2, h / 2, 0, 3);

		stbi_image_free(big);
	}

	w /= 2; // the images get scaled down
	h /= 2;

	// MERGE IMAGES

	int mw = w * 2; // final image is 2x2 smaller images
	int mh = h * 2;
	byte* merged = xmalloc(mw * mh * 3); // 3 chars per pixel (rgb)

	// each for copies line by line from one scaled down image into one quadrant of the final image
	for (int l = 0; l < h; l++) memcpy(&merged[(((l    ) * mw)    ) * 3], &imgs[0][l * w * 3], w * 3);
	for (int l = 0; l < h; l++) memcpy(&merged[(((l    ) * mw) + w) * 3], &imgs[1][l * w * 3], w * 3);
	for (int l = 0; l < h; l++) memcpy(&merged[(((l + h) * mw)    ) * 3], &imgs[2][l * w * 3], w * 3);
	for (int l = 0; l < h; l++) memcpy(&merged[(((l + h) * mw) + w) * 3], &imgs[3][l * w * 3], w * 3);

	for (int i = 0; i < 4; i++)
		free(imgs[i]);

	// WRITE MERGED IMAGE

	if (png) stbi_write_png(payload->paths + PATH_LEN * 4, mw, mh, 3, merged, 0);
	else     stbi_write_jpg(payload->paths + PATH_LEN * 4, mw, mh, 3, merged, jpg_quality);

	xfree(merged);
}

static bool file_exists(char* filename) {
	return access(filename, R_OK | W_OK) == 0;
}

static struct w* construct_job(char* path, char* blank, int from, int bx, int by, int x, int y, char* ext) {
	struct w *w = xmalloc_zero(sizeof(struct w));
	w->w.go = go;
	w->w.priority = 0;
	w->x = x;
	w->y = y;

	sprintf(w->paths + PATH_LEN * 0, "%s/images/0/%d/%d_%d.%s", path, from, bx+0, by+0, ext);
	sprintf(w->paths + PATH_LEN * 1, "%s/images/0/%d/%d_%d.%s", path, from, bx+1, by+0, ext);
	sprintf(w->paths + PATH_LEN * 2, "%s/images/0/%d/%d_%d.%s", path, from, bx+0, by+1, ext);
	sprintf(w->paths + PATH_LEN * 3, "%s/images/0/%d/%d_%d.%s", path, from, bx+1, by+1, ext);

	int bc = 0; // TODO we are potentionally calling 4 strcpys for nothing
	if (!file_exists(w->paths + PATH_LEN * 0)) { strcpy(w->paths + PATH_LEN * 0, blank); bc++; }
	if (!file_exists(w->paths + PATH_LEN * 1)) { strcpy(w->paths + PATH_LEN * 1, blank); bc++; }
	if (!file_exists(w->paths + PATH_LEN * 2)) { strcpy(w->paths + PATH_LEN * 2, blank); bc++; }
	if (!file_exists(w->paths + PATH_LEN * 3)) { strcpy(w->paths + PATH_LEN * 3, blank); bc++; }
	if (bc == 4) {
		free(w);
		return NULL; // don't merge 4 blanks
	}

	// we don't have to sprintf output path until we know that we can't skip
	sprintf(w->paths + PATH_LEN * 4, "%s/images/0/%d/%d_%d.%s", path, from-1, x, y, ext);

	return w;
}

void zoomout(struct work_queue* q, char* path, char* blank, int from, int* maxx, int* maxy, int* minx, int* miny) {
	*minx = *minx / 2 - 1; // round down is default
	*miny = *miny / 2 - 1;
	*maxx = (*maxx + 1) / 2 + 1; // the round up trick
	*maxy = (*maxy + 1) / 2 + 1;

	char* ext = png ? "png" : "jpg";
	int submitted = 0;

	char* out_dir = stk_printf("%s/images/0/%d", path, from-1);
	mkdir(out_dir, 0755);

	for (int x = *minx; x < *maxx; x++) {
		for (int y = *miny; y < *maxy; y++) {
			int bx = x * 2;
			int by = y * 2;

			struct w* w = construct_job(path, blank, from, bx, by, x, y, ext);
			if (w) {
				work_submit(q, &w->w);
				log("Submitted job %d.", ++submitted);
			}
		}
	}

	struct w *w;
	int finished = 0;
	while ((w = (struct w*) work_wait(q))) {
		log("Finished job (%d/%d).", ++finished, submitted);
		free(w);
	}
}

