CC = gcc

CFLAGS = -Wall -Wextra -g `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer

SRC = source/main.c source/helper.c
OBJ = $(SRC:.c=.o)

TARGET = game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

source/%.o: source/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f source/*.o $(TARGET)
