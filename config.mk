TARGET  = ff
WARNS   = -Wall -Wextra -Wno-pointer-sign
CFLAGS  = -flto=full -O3
LDFLAGS = -Wl,--gc-sections

CC = clang

# source directory
S = src

# objects directory
O = obj

