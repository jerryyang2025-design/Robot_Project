#pragma once

#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHSD.h>
#include <FEH.h>
#include <utils.h>

#define SPEED 35
#define delay 100

class Robot {
    private:
        const float left_opto_threshold = 3; // may need to change
        const float right_opto_threshold = 2.5;
        // servo min/max, light sensor thresholds, others
        const float blue_threshold = 2;
        const float red_threshold = 0.85;
        const int8_t check_time = 50; // miliseconds, for collision checks
        int16_t baseAngle = 90, joint1Angle = 180, joint2Angle = 10;
        int16_t angles[3] = {baseAngle, joint1Angle, joint2Angle};
        uint8_t rotateSpeed = 150, rotateIncrement = 10; // rotateSpeed: pause time between increments in milliseconds

    public:
        bool move_forward(float inches, int8_t early = 0, int8_t speed = SPEED); // refer to detect
        bool turn(int16_t degrees, int8_t direction, int8_t early = 0); // positive: turn right, negative: turn left
        bool detect(int8_t type); // 0 = none, 1 = light
        int8_t lightColor(); // 0 = no light, -1 = blue, 1 = red
        void hug(int8_t side); // 0 = front, 1 = right side, -1 = left side
        void sweep(float inches = 4, int8_t angle = 60, float gap = 0.4);
        void rotate(int8_t jointIndex, int16_t angle, boolean slow = false, int8_t jointIndex2 = 4, int16_t angle2 = 0); // base = 0, joint1 = 1, joint2 = 2
        void defaultArm();

        void stop(); // for testing only
};