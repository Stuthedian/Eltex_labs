CC=gcc
CFLAGS = -g -Wall
.PHONY : all
all : server client

objects = check.o server_helper.o server.o client.o

server: check.o server_helper.o server.o
	$(CC) $(CFLAGS) $^ -o $@ -pthread -lrt
	
client: check.o client.o
	$(CC) $(CFLAGS) $^ -o $@ 


$(objects): check.h
server.o server_helper.o: check.h server_helper.h

.PHONY: clean
clean:
	-rm server client $(objects) 2>/dev/null
	
.PHONY: rebuild
rebuild:
	make clean && make	
