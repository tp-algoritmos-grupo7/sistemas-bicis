CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -Iinclude
SRC = $(wildcard src/*.cpp) $(wildcard src/structures/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = sistema_bicis

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o src/structures/*.o $(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

.PHONY: all clean valgrind