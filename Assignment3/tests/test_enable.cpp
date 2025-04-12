/**
 * Tests to check intersection of enabling sets is done correctly.
 */
#include "../enable.hpp"
#include <iostream>
/**
 * Main Function.
 */

void intersect_test(Enable* A, Enable* B) {
    Enable* result = A->intersect(*B);
    std::cout << "Intersected A = " << A->toString() << " and B = " << B->toString() << " and got ";
    if(result != nullptr) {
        std::cout << result->toString() << std::endl;
        delete result;
    } else {
        std::cout << "<empty>" << std::endl;
    }
    delete A;
    delete B;
}

void intersect_always_x(Enable* A, Enable* B) {
    Enable* result = A->intersect(*B);
    std::cout << "Intersected A = " << A->toString() << " and B = " << B->toString() << " and got ";
    if(result != nullptr) {
        std::cout << result->toString() << std::endl;
        delete result;
    } else {
        std::cout << "<empty>" << std::endl;
    }
    delete B;
}

void intersect_x_always(Enable* A, Enable* B) {
    Enable* result = A->intersect(*B);
    std::cout << "Intersected A = " << A->toString() << " and B = " << B->toString() << " and got ";
    if(result != nullptr) {
        std::cout << result->toString() << std::endl;
        delete result;
    } else {
        std::cout << "<empty>" << std::endl;
    }
    delete A;
}

/**
 * NOTE: For tests doing intersections, it may seem counterintuitive to call
 * intersect_test() with the reverse order of Enabling functions, that is;
 *
 * To test the interval intersection method when given an upperbound
 * (i.e. interval->intersect(upper_bound)) we call:
 *
 * intersect_test(new UpperBound(), new Interval());
 *
 * NOT
 *
 * intersect_test(new Interval(), new UpperBound());
 *
 * This is due to the double dispatch pattern used to handle intersections.
 */
int main() {
    std::cout << "== Interval Tests ==" << std::endl;
    /**
     * Interval Creation tests
     */
    Interval* A = nullptr;
    std::cout << "== Tests creating invalid intervals ==" << std::endl;
    std::cout << "Attempt to create Interval [10, 5] (should throw exception)" << std::endl;
    try {
        A = new Interval(10, true, 5, false);
        delete A;
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "Attempt to create Interval (1, 1] (should throw exception)" << std::endl;
    try {
        A = new Interval(1, true, 1, false);
        delete A;
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    /**
     * Interval intersection tests
     */
    // Interval & UpperBound
    std::cout << "== Intersecting Interval and UpperBound ==" << std::endl;
    intersect_test(new UpperBound(10, false), new Interval(0, false, 4, true));
    intersect_test(new UpperBound(10, false), new Interval(2, false, 4, true));
    intersect_test(new UpperBound(10, false), new Interval(8, false, 11, true));
    intersect_test(new UpperBound(10, false), new Interval(8, false, 13, true));
    intersect_test(new UpperBound(10, false), new Interval(15, false, 18, true));
    // Interval & LowerBound
    std::cout << "== Intersecting Interval and LowerBound ==" << std::endl;
    intersect_test(new LowerBound(5, false), new Interval(0, false, 3, false));
    intersect_test(new LowerBound(5, false), new Interval(3, false, 7, true));
    intersect_test(new LowerBound(5, false), new Interval(5, false, 7, true));
    intersect_test(new LowerBound(5, false), new Interval(10, false, 13, true));
    // Interval & Interval
    std::cout << "== Intersecting Interval and Interval ==" << std::endl;
    intersect_test(new Interval(4, false, 10, false), new Interval(1, false, 3, false));
    intersect_test(new Interval(4, false, 10, false), new Interval(3, false, 5, false));
    intersect_test(new Interval(4, false, 10, false), new Interval(4, false, 5, false));
    intersect_test(new Interval(4, false, 10, false), new Interval(5, false, 9, false));
    intersect_test(new Interval(4, false, 10, false), new Interval(8, false, 11, true));
    intersect_test(new Interval(4, false, 10, false), new Interval(8, false, 12, true));
    intersect_test(new Interval(4, false, 10, false), new Interval(12, false, 15, true));
    /**
     * UpperBound Creation tests
     */
    std::cout << "== UpperBound Tests ==" << std::endl;
    std::cout << "Attempt to create UpperBound [0, 0) (should throw exception)" << std::endl;
    UpperBound* B = nullptr;
    try {
        B = new UpperBound(0, true);
        delete B;
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    /**
     * Upperbound intersection tests
     */
    // UpperBound & Interval
    std::cout << "== Intersecting UpperBound and Interval ==" << std::endl;
    intersect_test(new Interval(5, false, 15, true), new UpperBound(2, false));
    intersect_test(new Interval(5, false, 15, true), new UpperBound(5, false));
    intersect_test(new Interval(5, false, 15, true), new UpperBound(8, false));
    intersect_test(new Interval(5, false, 15, true), new UpperBound(14, false));
    intersect_test(new Interval(5, false, 15, true), new UpperBound(18, false));
    // UpperBound & LowerBound
    std::cout << "== Intersecting UpperBound and LowerBound ==" << std::endl;
    intersect_test(new LowerBound(6, false), new UpperBound(4, true));
    intersect_test(new LowerBound(6, false), new UpperBound(7, true));
    intersect_test(new LowerBound(6, false), new UpperBound(10, true));
    // UpperBound & UpperBound
    std::cout << "== Intersecting UpperBound and UpperBound ==" << std::endl;
    intersect_test(new UpperBound(10, false), new UpperBound(5, false));
    intersect_test(new UpperBound(10, false), new UpperBound(10, false));
    intersect_test(new UpperBound(10, false), new UpperBound(15, false));
    /**
     * LowerBound intersection tests
     */
    // LowerBound & Interval
    std::cout << "== LowerBound Tests ==" << std::endl;
    std::cout << "== Intersecting LowerBound and Interval ==" << std::endl;
    intersect_test(new Interval(3, false, 8, true), new LowerBound(2, false));
    intersect_test(new Interval(3, false, 8, true), new LowerBound(3, false));
    intersect_test(new Interval(3, false, 8, true), new LowerBound(6, false));
    intersect_test(new Interval(3, false, 8, true), new LowerBound(7, false));
    intersect_test(new Interval(3, false, 8, true), new LowerBound(10, false));
    // LowerBound & UpperBound
    std::cout << "== Intersecting LowerBound and UpperBound ==" << std::endl;
    intersect_test(new UpperBound(5, true), new LowerBound(2, false));
    intersect_test(new UpperBound(5, true), new LowerBound(4, false));
    intersect_test(new UpperBound(5, true), new LowerBound(6, false));
    // LowerBound & LowerBound
    std::cout << "== Intersecting LowerBound and LowerBound ==" << std::endl;
    intersect_test(new LowerBound(10, false), new LowerBound(5, false));
    intersect_test(new LowerBound(10, false), new LowerBound(10, false));
    intersect_test(new LowerBound(10, false), new LowerBound(15, false));
    return 0;
}