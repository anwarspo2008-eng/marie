CC = gcc
CFLAGS = -Wall -Wextra -I includes

SRC = src/main.c src/server.c src/auth.c src/Look.c
TARGET = marie

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean