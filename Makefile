.PHONY : all clean deepclean help

include config.mk

TARGET ?= ff
WARNS  ?= -Wall -Wextra

INCLUDE = -Ideps/stb/include

CFLAGS  += -std=gnu17 $(WARNS)
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

