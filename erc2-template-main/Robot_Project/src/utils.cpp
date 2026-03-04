#include <utils.h>
#include <Arduino.h>

#define EPSILON 1e-5

bool equals(float a, float b) {
    return abs(a - b) < EPSILON;
}

bool equals(const Position& a, const Position& b) {
    return equals(a.x, b.x) && equals(a.y, b.y) && equals(a.z, b.z);
}