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
        int baseAngle = 0, joint1Angle = 0, joint2Angle = 0;
        int angles[3] = {baseAngle, joint1Angle, joint2Angle};

        int rotateSpeed = 10, rotateIncrement = 10; // rotateSpeed: pause time between increments in milliseconds

        bool detect(int type); // 0 = no detection, 1 = microswitches, 2 = light
        void follow(FEHMotor motor1, FEHMotor motor2);
    public:
        void move_forward(int inches, int percent = SPEED, int early = 0); // 0 = no early exit, 1 = microswitches, 2 = light
        void turn(int degrees, int direction, int percent = SPEED); // positive: turn right, negative: turn left
        void controlledFollow(int inches, int direction = 0, int early = 0); // -1 = left bias, 1 = right bias; 0 = no early exit, 1 = microswitches, 2 = light
        void rotate(int jointIndex, int angle); // base = 0, joint1 = 1, joint2 = 2
        void defaultArm();
        void calibrate(); // set servo min/max, others?
        int lightColor(); // 0 = no light, -1 = blue, 1 = red
};

// will probably put this in another file
void navigate(); // course navigation and task completion here

// functions for each task