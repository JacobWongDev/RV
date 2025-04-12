CXX = g++
CXXFLAGS = -std=c++23 -g
BUILD_DIR = build
objects = efsm.o enable.o
programs = test_efsm1 test_efsm2 simple1 simple2 complex1 complex2 error_test test_enable

all: $(BUILD_DIR) $(programs)

# Example: g++ -std=c++23 -g -c efsm.cpp -o build/efsm.o
$(objects): %.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $(BUILD_DIR)/$@

# Example: g++ -std=c++23 -g -o build/complex1 tests/complex1.cpp build/efsm.o build/enable.o
$(programs): efsm.o enable.o
	$(CXX) $(CXXFLAGS) -o build/$@ tests/$@.cpp $(BUILD_DIR)/efsm.o $(BUILD_DIR)/enable.o

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: all clean