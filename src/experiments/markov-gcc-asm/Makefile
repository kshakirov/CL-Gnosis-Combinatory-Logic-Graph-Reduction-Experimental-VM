
CC = gcc
AS = gcc
CFLAGS = -Wall -O2 -g
LDFLAGS =

TARGET = markov
OBJS = main.o markov.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

markov.o: markov.S
	$(AS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
