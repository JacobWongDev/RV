#include <iostream>
#include "efsm.hpp"

/**
 * Macro used for testing.
 *
 * @param msg string used to identify the test
 * @param expr boolean result produced by function under test
 * @param expected boolean result expected
 */
#define ASSERT(msg, expr, expected) \
    if((static_cast <bool> (expr)) == (static_cast <bool> (expected))) { \
        std::cout << "TEST " << msg << " PASSED." << std::endl; \
    } else { \
        std::cerr << "TEST " << msg << " FAILED: Got " << static_cast <bool> (expr) \
                  << " but expected: " << static_cast <bool> (expected) << std::endl; \
    }

/**
 * Function used to print vector of non-empty language states.
 * @param run vector of non-empty language states
 */
void print_run(std::vector<SymbolicState*> run) {
    std::cout << "Run generated the following symbolic states:" << std::endl;
    if(run.empty()) {
        std::cout << "Empty! No runs found." << std::endl;
    } else {
        for(SymbolicState* current: run) {
            std::cout << "(" << current->getStringState() << ", "
                      << current->getInterval()->toString() << ")" << std::endl;
        }
    }
}