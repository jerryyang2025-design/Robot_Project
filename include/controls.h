#pragma once

#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHSD.h>
#include <FEH.h>
#include <utils.h>

#define SPEED 25

class Robot {
    private:
        const float left_opto_threshold = 3; // may need to change
        const float right_opto_threshold = 2.5;
        // servo min/max, light sensor thresholds, others
        int16_t baseAngle = 0, joint1Angle = 0, joint2Angle = 0;
        int16_t angles[3] = {baseAngle, joint1Angle, joint2Angle};

        int8_t rotateSpeed = 10, rotateIncrement = 10; // rotateSpeed: pause time between increments in milliseconds

        bool detect(int8_t type); // 0 = no detection, 1 = microswitches, 2 = light
        void follow(FEHMotor motor1, FEHMotor motor2);
    public:
        void move_forward(int8_t inches, int8_t percent = SPEED, int8_t early = 0); // 0 = no early exit, 1 = microswitches, 2 = light
        void turn(int16_t degrees, int8_t direction, int8_t percent = SPEED); // positive: turn right, negative: turn left
        void controlledFollow(int8_t inches, int8_t direction = 0, int8_t early = 0); // -1 = left bias, 1 = right bias; 0 = no early exit, 1 = microswitches, 2 = light
        void rotate(int8_t jointIndex, int16_t angle); // base = 0, joint1 = 1, joint2 = 2
        void defaultArm();
        void calibrate(); // set servo min/max, others?
        int8_t lightColor(); // 0 = no light, -1 = blue, 1 = red
};

// will probably put this in another file
void navigate(); // course navigation and task completion here

// functions for each task