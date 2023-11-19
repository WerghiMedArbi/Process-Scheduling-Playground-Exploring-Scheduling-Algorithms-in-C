CC = gcc
CFLAGS = -Wall -Wextra

all: process_generator fifo sjf rr priority multilevel

process_generator: Process_generator.c
	$(CC) $(CFLAGS) -o process_generator Process_generator.c

fifo: Politiques/FIFO.c
	$(CC) $(CFLAGS) -o fifo Politiques/FIFO.c

sjf: Politiques/SJF.c
	$(CC) $(CFLAGS) -o sjf Politiques/SJF.c

	
rr: Politiques/RR.c
	$(CC) $(CFLAGS) -o rr Politiques/RR.c
	
priority: Politiques/PRIORITY.c
	$(CC) $(CFLAGS) -o priority Politiques/PRIORITY.c

multilevel: Politiques/MULTILEVEL.c
	$(CC) $(CFLAGS) -o multilevel Politiques/MULTILEVEL.c

clean:
	rm -f process_generator fifo sjf rr priority multilevel
