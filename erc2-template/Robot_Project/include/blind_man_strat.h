#pragma once

#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHSD.h>
#include <FEH.h>
#include <utils.h>

#define SPEED 25
#define delay 100

class Robot {
    private:
        const float left_opto_threshold = 3; // may need to change
        const float right_opto_threshold = 2.5;
        // servo min/max, light sensor thresholds, others
        const float blue_threshold = 2;
        const float red_threshold = 0.85;
        const int8_t check_time = 50; // miliseconds, for collision checks

    public:
        bool move_forward(float inches, int8_t early = 0, int8_t speed = SPEED); // refer to detect
        bool turn(int16_t degrees, int8_t direction, int8_t early = 0); // positive: turn right, negative: turn left
        bool detect(int8_t type); // 0 = none, 1 = light
        int8_t lightColor(); // 0 = no light, -1 = blue, 1 = red
        void hug(int8_t side); // 0 = front, 1 = right side, -1 = left side
        void sweep(float inches = 4, int8_t angle = 60, float gap = 0.4);

        void stop(); // for testing only
};

void runCourse(Robot robot);