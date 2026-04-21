#pragma once

#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHSD.h>
#include <FEH.h>
#include <utils.h>
#include <radio.h>

#define SPEED 40
#define SPRINT 80
#define delay 50

class Robot {
    private:
        const bool debugMode = true;
        const uint8_t universalPause = 1;
        // servo min/max, light sensor thresholds, others
        int16_t baseAngle = 90, joint1Angle = 180, joint2Angle = 10;
        int16_t angles[3] = {baseAngle, joint1Angle, joint2Angle};
        const uint8_t rotateSpeed = 20, rotateIncrement = 50; // rotateSpeed: pause time between increments in milliseconds
        float redThreshold = 0.55;

        uint16_t clamp(float value, float min, float max);

        // music
        bool musicStarted = false;
        long musicStartTime = 0;
        const bool musicLoop = true;
        int currentIndex = 0;
        uint16_t currentFrameInIndex = 0;
        int previousFrames = 0;
        void musicPlayer();

        void test1();
        void test2();
        void test3();

    public:
        uint8_t currentCourse = 0; // default course A

        void initialize();
        bool move_forward(float inches, int8_t early = 0, bool backUp = false, int8_t speed = SPEED); // refer to detect
        bool turn(int16_t degrees, int8_t direction, int8_t early = 0); // positive: turn right, negative: turn left
        bool detect(int8_t type); // 0 = none, 1 = light
        int8_t lightColor(); // 0 = no light, -1 = blue, 1 = red
        void hug(int8_t side); // 0 = front, 1 = right side, -1 = left side
        void sweep(float inches = 4, int8_t angle = 60, float gap = 0.4);
        void rotate(int8_t jointIndex, int16_t angle, boolean slow = false, int8_t jointIndex2 = 4, int16_t angle2 = 0); // base = 0, joint1 = 1, joint2 = 2
        void defaultArm();
        int8_t lever();
        void Pause(uint16_t milli);

        // overloaded function, because the implementation feels cleaner (pain to call though, probably won't be used much)
        void rotate(int8_t joint[], int16_t angle[], int8_t size = 1, boolean slow = false);

        void sprint(float inches, int8_t highSpeed = SPRINT, int8_t lowSpeed = SPEED);

        void stop(const char* msg = "CheckPoint"); // for testing only
        void debugTest(uint8_t test);
};