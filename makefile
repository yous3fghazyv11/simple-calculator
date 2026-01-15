CC      = g++
CFLAGS  = -Wall -Wextra -std=c++23 -lreadline -lhistory
TARGET  = calc

SRCS    = calc.cpp parser.cpp token.cpp var.cpp utils.cpp raw_input.cpp
OBJS    = $(SRCS:.cpp=.o)

CONFIG_DIR := $(HOME)/.config/calc
CONFIG_FILE := $(CONFIG_DIR)/calc.conf

.PHONY: all clean run install uninstall

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

install: all
	@echo "→ Installing $(TARGET) to /usr/bin and creating config directory"
	sudo cp $(TARGET) /usr/bin/
	@mkdir -p "$(CONFIG_DIR)"
	@[ -f "$(CONFIG_FILE)" ] || echo "# calc configuration" > "$(CONFIG_FILE)"
	@echo "   Config file is at $(CONFIG_FILE)"

uninstall:
	@echo "→ Removing /usr/bin/$(TARGET) and config directory"
	@sudo rm -f /usr/bin/$(TARGET)
	@rm -rf "$(CONFIG_DIR)"

clean:
	rm -f $(TARGET) $(OBJS)
