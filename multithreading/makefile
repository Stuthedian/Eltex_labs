main : shop.o main.o 
	cc -D_REENTRANT  shop.o main.o  -o main -lpthread
main.o : main.c shop.h
shop.o : shop.c shop.h
