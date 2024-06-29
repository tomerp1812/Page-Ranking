# Define the compiler and compiler flags
CC = gcc
CFLAGS = -std=c11

# Define the output file
OUTPUT = main.out

# Define the source files
SRCS = graph.c solution.c thr_pool.c main.c

# Define the object files (replace .c with .o in SRCS)
OBJS = $(SRCS:.c=.o)

# The default target
all: $(OUTPUT)

# Link the object files to create the executable
$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(OBJS)

# Compile the source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(OUTPUT)

.PHONY: all clean
