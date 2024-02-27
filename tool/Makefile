# Define the target executable name, output directory, and object directory
TARGET=src
OUTDIR=dist
OBJDIR=$(OUTDIR)/objs
ARGS=playground
UNITY=unity
TEST_DIR=tests

# All source files inside src/
SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:src/%.c=$(OBJDIR)/%.o)

# Test source files (assuming they're in tests/ directory and use Unity)
TEST_SRCS=$(wildcard tests/*.c)
TEST_OBJS=$(TEST_SRCS:tests/%.c=$(OBJDIR)/%.o)

TEST_TARGET=$(OUTDIR)/interframe_test-runner
SOURCE_TARGET=$(OUTDIR)/interframe

# Define compiler and compiler flag variables (with debug)
CC=gcc
CFLAGS=-Wall -g -I$(TARGET) -I$(UNITY)

# Ensure the output and object directories exist
$(shell mkdir -p $(OUTDIR) $(OBJDIR))

# Target for compiling the main application
compile: $(OBJS)
	$(CC) -o $(SOURCE_TARGET) $^ $(CFLAGS)

# Target for running the main application
run: compile
	./$(SOURCE_TARGET) $(ARGS)

# Target for compiling and running the test code
test: $(TEST_OBJS)
	$(CC) -o $(TEST_TARGET) $^ $(CFLAGS) $(UNITY)/unity.c
	./$(TEST_TARGET)

# Target for cleaning up the build
clean:
	rm -rf $(OUTDIR)/* $(OBJDIR) $(TARGET)/*.o $(TEST_DIR)/*.o

debug: compile  # Target for debugging the main application
	lldb $(SOURCE_TARGET) # Use lldb to debug the main application

# Pattern rule for object files, adjusted to use the OBJDIR
$(OBJDIR)/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Pattern rule for test object files, adjusted to use the OBJDIR
$(OBJDIR)/%.o: tests/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Special targets
.PHONY: compile run test clean
