#!/bin/sh
zip --symlinks release.zip \
	run readme.md LICENSE docs/* util/* \
	src/fotograf.py src/imgmerge src/blank.png \
	src/web/* \
	src/fotograf_1.0.0/* \
	src/c/*.[ch] src/c/Makefile \
	src/c/C-Thread-Pool/thpool.[ch] \
	src/c/stb/stb_image.h \
	src/c/stb/stb_image_resize.h \
	src/c/stb/stb_image_write.h
