CC = gcc
CFLAGS = -Wall -Wextra -std=c11 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

OBJ = main.o helper.o
TARGET = program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)
