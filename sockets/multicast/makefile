CC=gcc
CFLAGS = -g -Wall -Wfatal-errors

targets = sender receiver

.PHONY: all
all: $(targets) 

objects = check.o sender.o receiver.o

sender: sender.o check.o 
	$(CC) $(CFLAGS) $^ -o $@
	
receiver: receiver.o check.o
	$(CC) $(CFLAGS) $^ -o $@
	
$(objects): check.h

.PHONY: clean
clean:
	-rm $(targets) $(objects) 2>/dev/null
	
.PHONY: rebuild
rebuild:
	make clean && make	
