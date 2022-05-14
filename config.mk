TARGET  = ff
WARNS   = -Wall -Wextra -Wno-pointer-sign
CFLAGS  = -flto=auto -O3
LDFLAGS = -Wl,--gc-sections

# source directory
S = src

# objects directory
O = obj

