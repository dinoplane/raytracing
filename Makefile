CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SRC = main.cpp color.h hittable.h hittable_list.h ray.h rtweekend.h sphere.h
EXECUTABLE = raytracer

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)