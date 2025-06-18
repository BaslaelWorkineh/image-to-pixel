# Image to Pixel Art Converter Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
INCLUDES = -Iinclude
LIBS = -lm

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Target executable
TARGET = $(BINDIR)/pixel-art-converter

# Default target
all: $(TARGET)

# Create directories if they don't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Build target executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LIBS)
	@echo "Build complete: $(TARGET)"

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Clean complete"

# Install (copy to /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installed to /usr/local/bin/"

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/pixel-art-converter
	@echo "Uninstalled from /usr/local/bin/"

# Create example images directory
examples-dir:
	mkdir -p examples

# Run tests (if you have test images)
test: $(TARGET) examples-dir
	@echo "Running basic tests..."
	@if [ -f "examples/test.jpg" ] || [ -f "examples/test.png" ]; then \
		echo "Testing with existing image..."; \
		./$(TARGET) examples/test.* examples/output_test.png; \
	else \
		echo "No test images found in examples/ directory"; \
		echo "Add a test image (test.jpg or test.png) to examples/ to run tests"; \
	fi

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the pixel art converter (default)"
	@echo "  clean    - Remove build files"
	@echo "  install  - Install to /usr/local/bin/"
	@echo "  uninstall- Remove from /usr/local/bin/"
	@echo "  debug    - Build with debug symbols"
	@echo "  test     - Run basic tests (requires test image in examples/)"
	@echo "  help     - Show this help message"

# Phony targets
.PHONY: all clean install uninstall test debug help examples-dir
