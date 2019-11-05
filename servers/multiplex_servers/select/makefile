VPATH = ..
CC=gcc
CFLAGS = -g -Wall
.PHONY: all
all: server

objects = check.o server.o handle_client.o

server: $(objects)
	$(CC) $(CFLAGS) $^ -o $@
	
$(objects): check.h
server.o handle_client.o: check.h handle_client.h

.PHONY: clean
clean:
	-rm server $(objects) 2>/dev/null
	
.PHONY: rebuild
rebuild:
	make clean && make	
