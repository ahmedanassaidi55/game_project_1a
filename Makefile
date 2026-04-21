CC := gcc
TARGET := game

SRC_DIR := source
IMPL_DIR := $(SRC_DIR)/implementation
INCLUDE_DIR := $(SRC_DIR)/includes
BUILD_DIR := build

PKG_CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf SDL2_mixer)
PKG_LIBS := $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)

CFLAGS := -Wall -Wextra -std=c11 $(PKG_CFLAGS) -I$(INCLUDE_DIR)
LDFLAGS := $(PKG_LIBS) -lm

SOURCES := \
	$(SRC_DIR)/main.c \
	$(IMPL_DIR)/helper.c \
	$(IMPL_DIR)/background.c \
	$(IMPL_DIR)/enigma.c \
	$(IMPL_DIR)/game.c \
	$(IMPL_DIR)/gameplay.c \
	$(IMPL_DIR)/joueur.c \
	$(IMPL_DIR)/ennemi.c \
	$(IMPL_DIR)/minimap_game.c \
	$(IMPL_DIR)/Puzzle.c
OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
