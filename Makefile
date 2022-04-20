.PHONY : all clean deepclean help

include config.mk

TARGET ?= ff
WARNS  ?= -Wall -Wextra -Wno-unused-parameter

INCLUDE = -Ideps/stb/include

CFLAGS  += -std=gnu17 $(WARNS)
LDFLAGS += -lm -lpthread

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean::
	$(RM) -- $(TARGET)

deepclean:: clean

include Makedeps
include Makerules

