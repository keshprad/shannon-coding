G++ = g++
FLAGS = -std=c++17
TARGET = main

all: $(TARGET)

$(TARGET): main.o
	$(G++) $(FLAGS) main.o -o $(TARGET)

main.o: main.cpp shannon.h trie.h
	$(G++) $(FLAGS) -c main.cpp

clean:
	rm *.o $(TARGET)