CXX = g++
CXXFLAGS = -std=c++23 -g
BUILD_DIR = build
objects = efsm.o
programs = trivial simple_dfa simple_nfa simple_efsm simple_nfa_ep complex_nfa complex_efsm error_test

all: $(BUILD_DIR) $(programs)

# Example: g++ -std=c++23 -g -c efsm.cpp -o build/efsm.o
$(objects): %.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $(BUILD_DIR)/$@

# Example: g++ -std=c++23 -g -o build/complex_efsm tests/complex_efsm.cpp build/efsm.o
$(programs): efsm.o
	$(CXX) $(CXXFLAGS) -o build/$@ tests/$@.cpp $(BUILD_DIR)/efsm.o

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: all clean