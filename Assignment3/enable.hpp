/**
 * File to define Enabling function types.
 * We want to support Enable functions in the form of inequalities.
 *
 * This file contains 3 types of objects to define inequalities: Interval, UpperBound and
 * LowerBound.
 *
 * Each of these Enabling functions is over the Natural numbers, so for instance
 *
 *  [3, 5] means {3, 4, 5}
 *
 * To simplify logic internally, we represent all enabling functions with inclusive bounds, meaning
 *
 * UpperBounds like [0, 8) become [0, 7]
 * LowerBounds like (5, ∞) become [6, ∞)
 * Intervals like (4, 10) become [5, 9]
 *
 * NOTE: We use the 'double dispatch' pattern here to simulate runtime dispatching (available in
 * Java, but not available in c++).
 *
 * Runtime dispatching is selecting which function to call based
 * on the type of the parameters at runtime
 */
#ifndef ENABLE_H_
#define ENABLE_H_

#include <sstream>

// Forward declarations
class Interval;
class UpperBound;
class LowerBound;

/**
 * Abstract class to represent an enabling function.
 */
class Enable {
public:
    virtual ~Enable() = default;
    virtual bool check_enable(unsigned int& r) = 0;
    /**
     * Returns an Enable object representing the intersection
     * with @param other. If intersection is the empty set,
     * return nullptr.
     */
    virtual Enable* intersect(Enable& other) = 0;
    /**
     * Returns an Enable object representing the intersection
     * with @param other. If intersection is the empty set,
     * return nullptr.
     */
    virtual Enable* intersect(Interval& other) = 0;
    /**
     * Returns an Enable object representing the intersection
     * with @param other. If intersection is the empty set,
     * return nullptr.
     */
    virtual Enable* intersect(LowerBound& other) = 0;
    /**
     * Returns an Enable object representing the intersection
     * with @param other. If intersection is the empty set,
     * return nullptr.
     */
    virtual Enable* intersect(UpperBound& other) = 0;
    /**
     * Shift the Enabling function by the specific amount
     *
     * Ex. [5, 9] shifted by 2 -> [7, 11]
     *
     * @param s shift by s
     */
    virtual void shift(unsigned int s) = 0;
    /**
     * Print out the interval in string format.
     *
     * TODO: This is used for debugging, so should remove later.
     */
    virtual std::string toString() = 0;
};

/**
 * Abstract class to represent an interval. Bounds can be inclusive or
 * exclusive.
 *
 * Ex. [1, 2]
 */
class Interval : public Enable {
public:
    Interval(unsigned int lower,
        bool lower_strict,
        unsigned int upper,
        bool upper_strict);
    bool check_enable(unsigned int& r) override;
    Enable* intersect(Enable& other) override;
    Enable* intersect(Interval& other) override;
    Enable* intersect(UpperBound& other) override;
    Enable* intersect(LowerBound& other) override;
    void shift(unsigned int s) override;
    std::string toString() override;
private:
    Interval(unsigned int lower, unsigned int upper);
    unsigned int upper;
    unsigned int lower;
    friend class UpperBound;
    friend class LowerBound;
};

/**
 * Abstract class to represent a lower bound. Bound can be inclusive or
 * exclusive.
 *
 * Ex. [8,∞)
 */
class LowerBound : public Enable {
public:
    LowerBound(unsigned int bound, bool strict);
    bool check_enable(unsigned int& r) override;
    Enable* intersect(Enable& other) override;
    Enable* intersect(Interval& other) override;
    Enable* intersect(UpperBound& other) override;
    Enable* intersect(LowerBound& other) override;
    void shift(unsigned int s) override;
    std::string toString() override;
private:
    LowerBound(unsigned int bound);
    unsigned int bound;
    friend class Interval;
    friend class UpperBound;
};

/**
 * Abstract class to represent a upper bound. Bound can be inclusive or
 * exclusive.
 *
 * Ex. [0, 9)
 */
class UpperBound : public Enable {
public:
    UpperBound(unsigned int bound, bool strict);
    bool check_enable(unsigned int& r) override;
    Enable* intersect(Enable& other) override;
    Enable* intersect(Interval& other) override;
    Enable* intersect(LowerBound& other) override;
    Enable* intersect(UpperBound& other) override;
    void shift(unsigned int s) override;
    std::string toString() override;
private:
    UpperBound(unsigned int bound);
    unsigned int bound;
    friend class Interval;
    friend class LowerBound;
};

#endif
