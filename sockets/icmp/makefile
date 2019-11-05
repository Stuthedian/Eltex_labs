CC=gcc
CFLAGS = -g -Wall -Wfatal-errors -Wsign-compare
.PHONY: all
all: pingpong

objects = check.o pingpong.o

pingpong: $(objects)
	$(CC) $(CFLAGS) $^ -o $@ 
	
$(objects): check.h

.PHONY: clean
clean:
	-rm pingpong $(objects) 2>/dev/null
	
.PHONY: rebuild
rebuild:
	make clean && make	
