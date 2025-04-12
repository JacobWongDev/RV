#include <iostream>

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
        std::cerr << "TEST " << msg << " FAILED: Got " << static_cast <bool> (expr) << " but expected: " << static_cast <bool> (expected) << std::endl; \
    }
