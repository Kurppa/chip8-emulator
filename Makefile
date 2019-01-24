clean: chip8 
	rm *.o
chip8: main.o chip8.o
	g++ main.o chip8.o -o chip8 -lsfml-graphics -lsfml-window -lsfml-system
main.o: main.cpp
	g++ -c main.cpp -o main.o
chip8.o: chip8.cpp
	g++ -c chip8.cpp -o chip8.o

