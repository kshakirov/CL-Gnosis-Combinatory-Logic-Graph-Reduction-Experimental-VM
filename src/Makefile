CC = gcc
CFLAGS = -std=c23 -Wall -Wextra -Wpedantic -Werror -O2
TARGET = experiment
SOURCES = main.c asm/app.S

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

