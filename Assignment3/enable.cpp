#include "enable.hpp"

/**
 * Intervals
 */

Interval::Interval(unsigned int lower, bool lower_strict, unsigned int upper, bool upper_strict) {
    if(upper_strict && upper == 0) throw std::runtime_error("Empty interval");
    this->upper = upper_strict ? upper - 1 : upper;
    this->lower = lower_strict ? lower + 1 : lower;
    if(this->upper < this->lower) throw std::runtime_error("Invalid interval!");
}

Interval::Interval(unsigned int lower, unsigned int upper) {
    if(upper < lower) throw std::runtime_error("Invalid interval!");
    this->upper = upper;
    this->lower = lower;
}

bool Interval::check_enable(unsigned int& r) {
    if(r >= lower && r <= upper) return true;
    return false;
}

Enable* Interval::intersect(Enable& other) {
    return other.intersect(*this);
}

Enable* Interval::intersect(Interval& other) {
    if(this->upper < other.lower || other.upper < this->lower) return nullptr;
    unsigned int upper, lower;
    lower = std::max(this->lower, other.lower);
    upper = std::min(this->upper, other.upper);
    return new Interval(lower, upper);
}

Enable* Interval::intersect(UpperBound& other) {
    unsigned int upper;
    upper = std::min(this->upper, other.bound);
    if(this->lower > upper) return nullptr;
    else return new Interval(this->lower, upper);
}

Enable* Interval::intersect(LowerBound& other) {
    unsigned int lower;
    lower = std::max(this->lower, other.bound);
    if(lower > this->upper) return nullptr;
    else return new Interval(lower, this->upper);
}

void Interval::shift(unsigned int s) {
    upper += s;
    lower += s;
}

std::string Interval::toString() {
    std::ostringstream oss;
    oss << "[" << lower << "," << upper << "]";
    return oss.str();
}

/**
 * LowerBounds
 */

LowerBound::LowerBound(unsigned int bound, bool strict) {
    this->bound = strict ? bound + 1 : bound;
}

LowerBound::LowerBound(unsigned int bound) {
    this->bound = bound;
}

bool LowerBound::check_enable(unsigned int& r) {
    if(r >= bound) return true;
    else return false;
}

Enable* LowerBound::intersect(Enable& other) {
    return other.intersect(*this);
}

Enable* LowerBound::intersect(Interval& other) {
    unsigned int lower;
    lower = std::max(this->bound, other.lower);
    if(lower > other.upper) return nullptr;
    else return new Interval(lower, other.upper);
}

Enable* LowerBound::intersect(UpperBound& other) {
    if(this->bound > other.bound) return nullptr;
    else return new Interval(this->bound, other.bound);
}

Enable* LowerBound::intersect(LowerBound& other) {
    return new LowerBound(std::max(this->bound, other.bound));
}

void LowerBound::shift(unsigned int s) {
    bound += s;
}

std::string LowerBound::toString() {
    std::ostringstream oss;
    oss << "[" << bound << ",∞)";
    return oss.str();
}

/**
 * UpperBounds
 */

UpperBound::UpperBound(unsigned int bound, bool strict) {
    if(strict && bound == 0) throw std::runtime_error("Empty interval");
    this->bound = strict ? bound - 1 : bound;
}

UpperBound::UpperBound(unsigned int bound) {
    this->bound = bound;
}

bool UpperBound::check_enable(unsigned int& r) {
    if(r <= bound)return true;
    else return false;
}

Enable* UpperBound::intersect(Enable& other) {
    return other.intersect(*this);
}

Enable* UpperBound::intersect(Interval& other) {
    unsigned int upper;
    upper = std::min(this->bound, other.upper);
    if(other.lower > upper) return nullptr;
    else return new Interval(other.lower, upper);
}

Enable* UpperBound::intersect(LowerBound& other) {
    if(this->bound < other.bound) return nullptr;
    else return new Interval(other.bound, this->bound);
}

Enable* UpperBound::intersect(UpperBound& other) {
    return new UpperBound(std::min(this->bound, other.bound));
}

void UpperBound::shift(unsigned int s) {
    bound += s;
}

std::string UpperBound::toString() {
    std::ostringstream oss;
    oss << "[0," << bound << "]";
    return oss.str();
}