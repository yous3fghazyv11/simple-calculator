CC      = g++
CFLAGS  = -Wall -Wextra -std=c++23 -Iinclude
LDFLAGS = -lreadline -lhistory

TARGET  = calc

SRC_DIR   = src
BUILD_DIR = build

SRCS = calc.cpp parser.cpp token.cpp var.cpp utils.cpp 

SRC_PATHS = $(addprefix $(SRC_DIR)/,$(SRCS))
OBJS      = $(addprefix $(BUILD_DIR)/,$(SRCS:.cpp=.o))

CONFIG_DIR := $(HOME)/.config/calc
CONFIG_FILE := $(CONFIG_DIR)/calc.conf

.PHONY: all clean run install uninstall

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	./$(TARGET)

install: all
	sudo cp $(TARGET) /usr/bin/
	mkdir -p "$(CONFIG_DIR)"
	[ -f "$(CONFIG_FILE)" ] || echo "# calc configuration" > "$(CONFIG_FILE)"

uninstall:
	sudo rm -f /usr/bin/$(TARGET)
	rm -rf "$(CONFIG_DIR)"

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
