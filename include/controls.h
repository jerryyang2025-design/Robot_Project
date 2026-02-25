#pragma once

#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHSD.h>
#include <FEH.h>
#include <utils.h>

#define SPEED 25

class Robot {
    private:
        const float left_opto_threshold = 3;
        const float right_opto_threshold = 2.5;
        // servo min/max, others

        bool detect(int type); // 0 = no detection, 1 = microswitches, 2 = light
    public:
        void move_forward(int inches, int percent = SPEED, int early = 0); // 0 = no early exit, 1 = microswitches, 2 = light
        void turn(int degrees, int direction, int percent = SPEED); // positive: turn right, negative: turn left
        void controlledFollow(int inches, int direction = 0, int early = 0); // -1 = left bias, 1 = right bias; 0 = no early exit, 1 = microswitches, 2 = light
        void rotate(int jointIndex, int angle); // base = 0, joint1 = 1, joint2 = 2
        void defaultArm();
        void calibrate();
        int lightColor(); // 0 = no light, -1 = blue, 1 = red
};

// will probably put this in another file
void navigate(); // course navigation and task completion here

// functions for each task