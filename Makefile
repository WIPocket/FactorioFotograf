SOURCES = imgmerge.c

GCC_WFLAGS = -Wall -Wextra -Wundef -Wshadow -Wunreachable-code -Winit-self -Wuninitialized -Wno-misleading-indentation
GCC_FFLAGS = -fdata-sections -ffunction-sections
GCC_FLAGS = $(GCC_WFLAGS) $(GCC_FFLAGS) -std=gnu11 -Ofast
GCC_LIB = -lm
GCC_INCLUDES = -Istb/

GCC_FULL = $(GCC_INCLUDES) $(GCC_FLAGS) $(GCC_LIB) -DBUILD_NAME=\"$(BUILD_NAME)\"

build:
	gcc $(SOURCES) $(GCC_FULL) -o imgmerge -Wl,--gc-sections
	@echo Build succeeded
