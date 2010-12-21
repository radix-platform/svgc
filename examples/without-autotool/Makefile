
all: main

main: main.o
	gcc `svgc-config --ldflags` -o main main.o `svgc-config --libs` -lm

main.o: main.c
	gcc `svgc-config --cflags` -c main.c -o main.o 


clean:
	rm -f *.o main M.svg
