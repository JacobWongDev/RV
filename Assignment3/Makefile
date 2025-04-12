CXX = g++
CXXFLAGS = -std=c++23 -DPROD
BUILD_DIR = prod
objects = monitor.o enable.o
programs = monitor1 monitor2 monitor3 monitor4 monitor5

all: $(BUILD_DIR) $(programs)

# Example: g++ -std=c++23 -g -c monitor.cpp -o build/monitor.o
$(objects): %.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $(BUILD_DIR)/$@

# Example: g++ -std=c++23 -g -o build/complex1 tests/complex1.cpp build/monitor.o build/enable.o
$(programs): monitor.o enable.o
	$(CXX) $(CXXFLAGS) -o prod/$@ tests/$@.cpp $(BUILD_DIR)/monitor.o $(BUILD_DIR)/enable.o

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: all clean