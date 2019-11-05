server : message_type.o Linked_List.o server.o
	gcc message_type.o Linked_List.o server.o  -o server -Wall
Linked_List.o : Linked_List.h Linked_List.c
message_type.o : message_type.h message_type.c
server.o : server.c

.PHONY: clean
clean:
	rm *.o
