main:
	g++ -g -c main.cpp -Wall -Wextra
	g++ -g main.o -o minesweeper -lsfml-graphics -lsfml-window -lsfml-system
	rm main.o