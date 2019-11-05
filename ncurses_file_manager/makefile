main : panel.o main.o 
	cc -D_REENTRANT  panel.o main.o  -o main -lncurses -lpthread
main.o : main.c panel.h
list.o : panel.c panel.h
