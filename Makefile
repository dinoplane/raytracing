CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SRC = main.cpp
EXECUTABLE = raytracer

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)