# Makefile

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall `pkg-config --cflags gtk+-3.0`

# Linker flags
LDFLAGS = `pkg-config --libs gtk+-3.0`

# Source files
SRC = Main.c process_generator.c callbacks.c Politiques/FIFO.c Politiques/SRT.c Politiques/PRIORITY.c Politiques/RR.c Politiques/MULTILEVEL.c

# Object files
OBJ = $(SRC:.c=.o)

# Executable
EXECUTABLE = start

# Targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

callbacks.o: Politiques/FIFO.o Politiques/SRT.o Politiques/PRIORITY.o Politiques/RR.o Politiques/MULTILEVEL.o

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

Politiques/FIFO.o: Politiques/FIFO.c
	$(CC) -c $< -o $@ $(CFLAGS)
Politiques/SRT.o: Politiques/SRT.c
	$(CC) -c $< -o $@ $(CFLAGS)
Politiques/PRIORITY.o: Politiques/PRIORITY.c
	$(CC) -c $< -o $@ $(CFLAGS)
Politiques/RR.o: Politiques/RR.c
	$(CC) -c $< -o $@ $(CFLAGS)
Politiques/MULTILEVEL.o: Politiques/MULTILEVEL.c
	$(CC) -c $< -o $@ $(CFLAGS)
	
clean:
	rm -f $(OBJ) $(EXECUTABLE)

