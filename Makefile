G++ = g++
FLAGS = -std=c++17 -g
TARGET = main

all: $(TARGET)

$(TARGET): main.o
	$(G++) -o $(TARGET) main.o

main.o: main.cpp shannon.h trie.h
	$(G++) $(FLAGS) -c main.cpp

clean:
	rm *.o $(TARGET)