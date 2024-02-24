# Define compiler and compiler flag variables
CC=gcc
CFLAGS=-Wall -g

# Define the target executable name and the output directory
TARGET=main
OUTDIR=dist
ARGS=playground

# default rule to build the target executable
all: $(OUTDIR)/$(TARGET)

# ensure the output directory exists
$(OUTDIR):
	@mkdir -p $(OUTDIR)

# build the target executable
$(OUTDIR)/$(TARGET): $(TARGET).c | $(OUTDIR)
	@$(CC) $(CFLAGS) -o $@ $(TARGET).c

# clean
clean:
	@rm -rf $(OUTDIR)

# run the target executable and pass arguments to it
run: $(OUTDIR)/$(TARGET)
	@./$< $(ARGS)
