// This file just holds the STB implementations
// so that is doesn't have to be recompiled when img.c
// changes

#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

