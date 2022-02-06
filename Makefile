.PHONY : all clean deepclean help

include config.mk

TARGET ?= ff
WARNS  ?= -Wall -Wextra -Wno-unused-parameter

INCLUDE = -Ideps/stb/include

CFLAGS  += -std=gnu17 $(WARNS) -finline-limit=5000
LDFLAGS += -lm -lpthread

CFLAGS  += `pkgconf --cflags libucw libucw-json`
LDFLAGS += `pkgconf --libs   libucw libucw-json`

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean::
	$(RM) -- $(TARGET)

deepclean:: clean

help:
	@cat readme.md

include Makerules

