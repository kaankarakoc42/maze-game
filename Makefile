# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files (created from the source files)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Final executable
EXEC = $(BUILD_DIR)/game.exe

# Makefile targets
.PHONY: all clean run
# Default target - builds the executable
all: $(EXEC)

$(EXEC): $(SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) $(SOURCES) -o $(EXEC) -g -mconsole

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)


# Run the compiled executable
run: $(EXEC)
	./$(EXEC)
