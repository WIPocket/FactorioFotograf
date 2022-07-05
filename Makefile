.PHONY : all clean deepclean help

include config.mk

TARGET ?= ff

VERSION ?= 2.2
COMMIT  ?= $(shell git rev-parse --short HEAD)

INCLUDE = -Ideps/stb/include

WARNS   ?= -Wall -Wextra
CFLAGS  += -std=gnu17 $(WARNS) -DVERSION="\"$(VERSION)\"" -DCOMMIT="\"$(COMMIT)\""
LDFLAGS += -lm -lpthread

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

release: $(TARGET)
	strip $(TARGET)
	md5sum $(TARGET) > $(TARGET).md5

clean::
	$(RM) -- $(TARGET)

deepclean:: clean

include Makedeps
include Makerules

