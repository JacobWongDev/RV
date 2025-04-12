#include <iostream>
#include "monitor.hpp"

#ifndef TEST_H_
#define TEST_H_

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
 * Macro used for testing.
 *
 * @param msg string used to identify the test
 * @param expr VERDICT produced by function under test
 * @param expected VERDICT expected
 */
#define ASSERT_VERDICT(msg, expr, expected) \
    if((expr) == (expected)) { \
        std::cout << "TEST " << msg << " PASSED." << std::endl; \
    } else { \
        std::cerr << "TEST " << msg << " FAILED: Got " << static_cast <int> (expr) \
                << " but expected: " << static_cast <int> (expected) << std::endl; \
    }

#endif
