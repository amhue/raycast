all: main.o
	g++ main.o -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

main.o: ./src/main.cpp
	g++ -ggdb -Wall -Wextra -c ./src/main.cpp -o main.o
