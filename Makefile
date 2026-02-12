CC = gcc
CFLAGS = -Wall
TARGET = h1-counter

all: $(TARGET)

$(TARGET): h1-counter.c
	$(CC) $(CFLAGS) -o $(TARGET) h1-counter.c

clean:
	rm -f $(TARGET)
