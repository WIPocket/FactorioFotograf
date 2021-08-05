CFLAGS ?= -O3 -march=native -pipe
WARNS  += -Wall -Wunused-parameter -Wshadow -Wundef -Wunreachable-code -Wno-misleading-indentation

LDLIBS += -lm
CFLAGS += -Istb/

CC ?= gcc

TARGET = imgmerge

all: $(TARGET)

clean::
	rm -f $(TARGET)

clear: clean



DEPS = $(wildcard *.h)
SRCS = $(wildcard *.c)
OBJS = $(sort $(SRCS:.c=.o))

%.o: %.c $(DEPS)
	@echo CC $@
	@$(CC) $(WARNS) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@echo LD $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

clean::
	rm -f $(OBJS)
