client : server/message_type.o window.o client.o 
	cc -D_REENTRANT server/message_type.o window.o client.o  -o client -Wall -lncurses -lpthread
window.o : window.c window.h
client.o : client.c window.h

.PHONY: clean
clean:
	rm *.o

