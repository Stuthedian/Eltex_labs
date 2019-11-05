CC=gcc
CFLAGS = -g -Wall -Wfatal-errors -Wsign-compare
.PHONY: all
all: server client

objects = check.o server.o client.o udp_dgram.o

server: check.o server.o
	$(CC) $(CFLAGS) $^ -o $@
	
client: check.o client.o udp_dgram.o
	$(CC) $(CFLAGS) $^ -o $@ 
	
$(objects): check.h
udp_dgram.o: udp_dgram.h

.PHONY: clean
clean:
	-rm server client $(objects) 2>/dev/null
	
.PHONY: rebuild
rebuild:
	make clean && make	
