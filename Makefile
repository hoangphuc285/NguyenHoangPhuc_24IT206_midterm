CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -D_DEFAULT_SOURCE
TARGET = ls_custom

SRCS = main.c options.c entry.c sort.c display.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
