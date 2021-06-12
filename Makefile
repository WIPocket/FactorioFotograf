SRC = .

SOURCES = imgmerge.c

GCC_WFLAGS = -Wall -Wextra -Wundef -Wshadow -Wunreachable-code -Winit-self -Wuninitialized -Wno-misleading-indentation
GCC_FFLAGS = -fdata-sections -ffunction-sections
GCC_FLAGS = $(GCC_WFLAGS) $(GCC_FFLAGS) -std=gnu11 -O0 -g
GCC_LIB = -lm
GCC_INCLUDES = -Istb/

NAME = imgmerge
BUILD_NAME = $(shell git rev-parse --short HEAD)
BUILD_TIME = $(shell date +'%H%M%S_%d%m%y')
BUILD_DIR = .

GCC_FULL = $(GCC_INCLUDES) $(GCC_FLAGS) $(GCC_LIB) -DBUILD_NAME=\"$(BUILD_NAME)\"

build:
	gcc $(SOURCES) $(GCC_FULL) -o $(BUILD_DIR)/$(NAME) -Wl,--gc-sections
	@echo Build succeeded
