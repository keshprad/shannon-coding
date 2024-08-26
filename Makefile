G++ = g++
FLAGS = -std=c++17 -g
TARGET = main

all: $(TARGET)

$(TARGET): main.o symbol_code.o
	$(G++) -o $(TARGET) main.o symbol_code.o

main.o: main.cpp shannon.h trie.h symbol_code.h
	$(G++) $(FLAGS) -c main.cpp

symbol_code.o: symbol_code.cpp symbol_code.h
	$(G++) $(FLAGS) -c symbol_code.cpp

clean:
	rm *.o $(TARGET)