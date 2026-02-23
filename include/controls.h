#pragma once

#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHSD.h>
#include <FEH.h>

#define LEFT_SPEED 25
#define RIGHT_SPEED 25

class driveTrain {
    private:

    public:
        void move_forward(int percent, int inches);
        void turn(int percent, int degrees, int direction); // positive: turn right, negative: turn left
};