#pragma once

struct Position { // is the RAM really so small that standard library vectors and array don't work?
    float x, y, z; // this is so much more scuffed now

    Position(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

bool equals(float a, float b);

bool equals(const Position& a, const Position& b);