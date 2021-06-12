#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

int main(int argc, char* filenames[]) {
	if (argc != 6) { printf("Usage: ./imgmerge img1.png img2.png img3.png img4.png outputimage.png\n"); exit(1); }

	//
	// LOAD & SHRINK IMAGES
	//

	int w, h, n;
	unsigned char *imgs[4];
	for (int i = 0; i < 4; i++) {
		int xn, yn;
		unsigned char *big = stbi_load(filenames[i + 1], &xn, &yn, &n, 4);
		if (i == 0) { // on first iter set the expected image size
			w = xn;
			h = yn;
		} else { // on later iters check that image size is correct
			if (xn != w || yn != h) {
				printf("image %s is the wrong size %dx%d instead of %dx%d\n", filenames[i], xn, yn, w, h);
				exit(1);
			}
		}
		imgs[i] = malloc(w * h);
		stbir_resize_uint8( big, w, h, 0, imgs[i], w / 2, h / 2, 0, 4);
		printf("loaded %s: w=%d, h=%d, n=%d\n", filenames[i + 1], w, h, n);
	}
	w /= 2;
	h /= 2;

	//
	// MERGE IMAGES
	//

	int mw = w * 2;
	int mh = h * 2;
	unsigned char merged[mw * mh * 4]; // 4 chars per pixel (rgba)

	for (int l = 0; l < h; l++) { memcpy(&merged[(((l    ) * mw)    ) * 4], &imgs[0][l * w * 4], w * 4); } printf("merged %s\n", filenames[1]);
	for (int l = 0; l < h; l++) { memcpy(&merged[(((l    ) * mw) + w) * 4], &imgs[1][l * w * 4], w * 4); } printf("merged %s\n", filenames[2]);
	for (int l = 0; l < h; l++) { memcpy(&merged[(((l + h) * mw)    ) * 4], &imgs[2][l * w * 4], w * 4); } printf("merged %s\n", filenames[3]);
	for (int l = 0; l < h; l++) { memcpy(&merged[(((l + h) * mw) + w) * 4], &imgs[3][l * w * 4], w * 4); } printf("merged %s\n", filenames[4]);

	//
	// WRITE MERGED IMAGE
	//

	stbi_write_png(filenames[5], mw, mh, 4, merged, 0);
	return 0;
}

