#include <robot.h>

// motors
FEHMotor left_motor(FEHMotor::Motor2, 9);
FEHMotor right_motor(FEHMotor::Motor0, 9);

// sensors
DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin9);
AnalogInputPin light_sensor(FEHIO::Pin14);

// servos
FEHServo base(FEHServo::Servo0);
FEHServo joint1(FEHServo::Servo1);
FEHServo joint2(FEHServo::Servo2);
FEHServo servos[3] = {base, joint1, joint2};

/*
Helper function to catch stray values from exceeding hardware limits

single function, not worth bringing the utils file out of retirement
*/
uint16_t Robot::clamp(float value, float min, float max) {
    if (value > max) {
        return max;
    } else if (value < min) {
        return min;
    } else {
        return value;
    }
}

/*
Runs the initial starting sequence
*/
void Robot::initialize() {
    // defaultArm();
    rotate(2, -90, true); // to prevent arm from poking out past size limit
    rotate(0, -90, true);
    rotate(1, -90, true);
    if (!debugMode) {
        RCS.InitializeTouchMenu("0300G9LQG");
        currentCourse = RCS.CurrentRegionLetter() - 65; // for small adjustments to account for slight differences in each course
    }

    WaitForFinalAction();
    
    if (!debugMode) {
        while (!detect(1));
    }
    // for (int i = 0; i < 20; i++) { // probably not a good idea since it depends on where we initially put it
    //     float test = light_sensor.Value(); // so might remove
    //     if (test > redThreshold) {
    //         redThreshold = test;
    //     }
    //     Pause(universalPause);
    // }

    musicStarted = true;
    musicStartTime = millis();
}

/*
Drive the robot forwards or backwards a specified distance while optionally checking for encoder stall or light detection
*/
bool Robot::move_forward(float inches, int8_t early, bool backUp, int8_t speed) {
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    int8_t percent = speed;
    // float adjustment = 1.03;
    float adjustment = 1.0f; // to account for physical wheel misalignment

    float counts = inches * 40.5f;

    if (counts < 0) {
        counts *= -1;
        percent = -speed;
    }

    right_motor.SetPercent(adjustment * percent);
    left_motor.SetPercent(percent);

    bool stopped = false;
    int stall = 0;

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts && !detect(early)) {
        int currentLeftCounts = left_encoder.Counts();
        int currentRightCounts = right_encoder.Counts();

        Pause(universalPause);

        if (fabs((left_encoder.Counts() - right_encoder.Counts())) / 40.5f > 0.065) { // a little more than half a degree, around 2-3 encoder counts
            float correction = (left_encoder.Counts() - right_encoder.Counts()) / 300.0f; // random value for now, needs tuning
            left_motor.SetPercent(clamp(percent * (1 + correction), -100, 100)); // I have no idea why the signs are like this, feels backwards but it works
            right_motor.SetPercent(clamp(percent * (1 - correction), -100, 100));
        } else { // ensures it won't over correct and just wobble back and forth
            right_motor.SetPercent(percent);
            left_motor.SetPercent(percent);
        }

        if (early == 2 && left_encoder.Counts() > 10 && right_encoder.Counts() > 10) { // ugly type of nesting that happens when you don't plan ahead or refactor code:
            if (left_encoder.Counts() == currentLeftCounts && right_encoder.Counts() == currentRightCounts) {
                stall++;
                if (stall > 250) {
                    stopped = true;
                    if (!backUp) { // default false since slipping causes inconsistencies, so stopping is enough, no need to move back based on luck
                        break;
                    }
                    if (inches < 0) {
                        move_forward(2);
                    } else if (inches > 0) {
                        move_forward(-2);
                    }
                    break;
                }
            } else {
                stall = 0;
            }
        }
    }
    if (detect(early)) {
        stopped = true;
    }

    right_motor.Stop();
    left_motor.Stop();
    return stopped;
}

/*
Turns the robot left or right a specified angle
*/
bool Robot::turn(int16_t degrees, int8_t direction, int8_t early) { // positive: turn right, negative: turn left
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    int8_t percent = SPEED;
    float adjustment = 1.0f;

    /*
    technically the radius (track width / 2) isn't entirely accurate since the wheels are wonky, kind of just an average, really between 3.7 - 3.5
    I don't feel like measuring the exact angle the wheel is attached at, tracking it at any given time, and calculating the exact desired encoder count
    kind of just going to have to put up with a slight inaccuracy, or figure out a way to physically straighten the wheels despite the epoxy
    */
    const float radius = 3.67; // inches
    const float pi = 3.141592653589793238462643383;

    float counts = radius * 40.5f * degrees * pi / 180.0f;

    right_motor.SetPercent(-direction * percent);
    left_motor.SetPercent(direction * percent);

    bool stopped = false;

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts * adjustment) { // && !detect(early)) {
        Pause(universalPause);

        if (fabs((left_encoder.Counts() - right_encoder.Counts())) / 40.5f > 0.065) {
            float correction = (left_encoder.Counts() - right_encoder.Counts()) / 400.0f; // random value for now, needs tuning
            left_motor.SetPercent(clamp(direction * percent * (1 + correction), -100, 100));
            right_motor.SetPercent(clamp(-direction * percent * (1 - correction), -100, 100));
        } else { // ensures it won't over correct and just wobble back and forth
            right_motor.SetPercent(-direction * percent);
            left_motor.SetPercent(direction * percent);
        }
    }
    if (detect(early)) {
        stopped = true;
    }

    right_motor.Stop();
    left_motor.Stop();
    return stopped;
}

/*
Was meant to have a bunch of checks, ended up only being used to check light detection
*/
bool Robot::detect(int8_t type) { // 0 = none, 1 = light
    if (type == 0) {
        return false;
    }
    bool detected = false;
    if (type == 1) { // so I was going to have more, hence an int instead of bool, but ended up not doing that
        if (lightColor() != 0) {
            detected = true;
        }
    }
    return detected;
}

/*
Detects and returns the light color
*/
int8_t Robot::lightColor() { // 0 = no light, -1 = blue, 1 = red
    if (light_sensor.Value() <= redThreshold) {
        return 1;
    } else if (light_sensor.Value() < 1 && light_sensor.Value() > redThreshold) {
        return -1;
    } else {
        return 0;
    }
} // values were closer than expected even with color filter, might just have to accept it, too late to fix

/*
Stops the robot for adjustment during test runs for debugging
*/
void Robot::stop(const char* msg) {
    if (!debugMode) {
        return;
    }
    int x, y;
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Paused");
    LCD.Write("\nStep: ");
    LCD.WriteLine(msg);
    LCD.WriteLine();
    LCD.WriteLine("Touch the screen to continue");
    while(!LCD.Touch(&x,&y)) { //Wait for screen to be pressed
        Pause(universalPause);
    }
    while(LCD.Touch(&x,&y)) { //Wait for screen to be unpressed
        Pause(universalPause);
    }
    LCD.WriteLine("3");
    Pause(1000);
    LCD.WriteLine("2");
    Pause(1000);
    LCD.WriteLine("1");
    Pause(1000);
}

/*
Hugs the robot to the wall on either side to reorient itself
*/
void Robot::hug(int8_t side) { // 0 = front, 1 = right side, -1 = left side
    turn(90, -side);
    Pause(delay);
    move_forward(-12, 2);
    Pause(delay);
    move_forward(1);
    Pause(delay);
    turn(90, side);
}

/*
{Retired function} Sweeps the robot back and forth until it finds the light
*/
void Robot::sweep(float inches, int8_t angle, float gap) {
    if (detect(1)) {
        return;
    }
    turn(angle / 2, 1, 1);
    for (int i = 0; i < inches / gap; i++) {
        move_forward(gap, 1);
        bool found = turn(angle, (i % 2) * 2 - 1, 1);
        if (found) {
            break;
        }
    }
}

/*
Rotate up to two arm segments at a time, optionally performing linear interpolation (LERP) to rotate them at a slower speed
*/
void Robot::rotate(int8_t joint, int16_t angle, boolean slow, int8_t joint2, int16_t angle2) {
    int16_t moveAngle = angle;
    int16_t moveAngle2 = angle2;
    if (joint == 0) {
        moveAngle = angle + 90;
    } else if (joint == 1) {
        moveAngle = angle + 110;
    } else if (joint == 2) {
        moveAngle = 100 - angle;
    }

    if (joint2 == 0) {
        moveAngle2 = angle2 + 90;
    } else if (joint2 == 1) {
        moveAngle2 = angle2 + 110;
    } else if (joint2 == 2) {
        moveAngle2 = 100 - angle2;
    }
    int8_t steps = rotateIncrement;
    if (!slow) {
        steps = 1;
    }
    float increment = 1.0f * (moveAngle - angles[joint]) / steps;

    float increment2 = moveAngle2;
    if (joint2 != 4) {
        increment2 = 1.0f * (moveAngle2 - angles[joint2]) / steps;
    }
    for (int i = 0; i < steps; i++) {
        servos[joint].SetDegree(clamp(angles[joint] + increment * (i + 1), 0, 180));

        if (joint2 != 4) {
            servos[joint2].SetDegree(clamp(angles[joint2] + increment2 * (i + 1), 0, 180));
        }
        Pause(rotateSpeed);
    }
    angles[joint] = moveAngle;
    if (joint2 != 4) {
        angles[joint2] = moveAngle2;
    }
}

/*
Overloaded rotate function to accept an array of values for a more elegant implementation

random complaint, why can we not use any of the standard libraries, I hate raw C arrays
*/
void Robot::rotate(int8_t joint[], int16_t angle[], int8_t size, boolean slow) { // overloaded function, because the implementation feels cleaner (pain to call though, probably won't be used much)
    int16_t moveAngle[size];
    int8_t steps = rotateIncrement;
    if (!slow) {
        steps = 1;
    }
    float increment[size];
    for (int i = 0; i < size; i++) {
        if (joint[i] == 0) {
            moveAngle[i] = angle[i] + 90;
        } else if (joint[i] == 1) {
            moveAngle[i] = angle[i] + 110;
        } else if (joint[i] == 2) {
            moveAngle[i] = 100 - angle[i];
        }
        increment[i] = 1.0f * (moveAngle[i] - angles[joint[i]]) / steps;
    }

    for (int i = 0; i < steps; i++) {
        for (int j = 0; j < size; j++) {
            servos[joint[j]].SetDegree(clamp(angles[joint[j]] + increment[j] * (i + 1), 0, 180));
        }
        Pause(rotateSpeed);
    }
    for (int i = 0; i < size; i++) {
        angles[joint[i]] = moveAngle[i];
    }
}

/*
Moves the arm back to a default position when not being used to avoid interferring with navigation
*/
void Robot::defaultArm() {
    // rotate(0, 0);
    // rotate(1, 90);
    // rotate(2, 90);

    servos[1].SetDegree(180);
    servos[0].SetDegree(90);
    servos[2].SetDegree(10);
    angles[0] = 90;
    angles[1] = 180;
    angles[2] = 10;
}

/*
Maps the RCS lever value to a more useful value and allows the value to be read from another file
*/
int8_t Robot::lever() {
    if (debugMode) {
        return 1;
    }
    return 2 - RCS.GetLever();
}

/*
Drives the robot a specified distance while incrementing velocity in a parabolic arc to allow for faster speeds more smoothly
*/
void Robot::sprint(float inches, int8_t highSpeed, int8_t lowSpeed) {
    if (inches < 4.5) {
        move_forward(inches, 2);
        return;
    }

    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    float percent = lowSpeed;
    // float adjustment = 1.03;
    float adjustment = 1.0f; // to account for physical wheel misalignment

    float counts = inches * 40.5f;

    if (counts < 0) {
        counts *= -1;
        percent = -lowSpeed;
    }

    right_motor.SetPercent(adjustment * percent);
    left_motor.SetPercent(percent);

    int stall = 0;
    float increments;
    // int8_t increments = -8 * (highSpeed - lowSpeed) / (counts * counts) * currentDistance + 4 * (highSpeed - lowSpeed) / counts; // derivative of parabola
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts) {
        int currentLeftCounts = left_encoder.Counts();
        int currentRightCounts = right_encoder.Counts();
        float averageCounts = (currentLeftCounts + currentRightCounts) / 2.;
        increments = -8.0f * (highSpeed - lowSpeed) / (counts * counts) * averageCounts + 4.0f * (highSpeed - lowSpeed) / counts;
        if (percent < 0) {
            percent += -increments;
        } else {
            percent += increments;
        }
        right_motor.SetPercent(percent);
        left_motor.SetPercent(percent);

        Pause(universalPause);

        // if (fabs((left_encoder.Counts() - right_encoder.Counts())) / 40.5f > 0.065) {
        //     float correction = (left_encoder.Counts() - right_encoder.Counts()) / 300.0f; // random value for now, needs tuning
        //     left_motor.SetPercent(clamp(percent * (1 + correction), -100, 100));
        //     right_motor.SetPercent(clamp(percent * (1 - correction), -100, 100));
        // } else { // ensures it won't over correct and just wobble back and forth
        //     right_motor.SetPercent(percent);
        //     left_motor.SetPercent(percent);
        // }
        
        if (left_encoder.Counts() == currentLeftCounts && right_encoder.Counts() == currentRightCounts) {
            if (left_encoder.Counts() > 10 && right_encoder.Counts() > 10) { // makes sure it doesn't include the initial stall, just in case
                stall++;
                if (stall > 250) {
                    break;
                }
            } else {
                stall = 0;
            }
        }
    }

    right_motor.Stop();
    left_motor.Stop();
}

/*
Pauses code execution while continuously playing music in the background
*/
void Robot::Pause(uint16_t milli) {
    int increments = milli / universalPause;
    long timeNow = millis();
    for (int i = 0; i < increments; i++) {
        musicPlayer();
        if (millis() - timeNow >= milli) { // to account for code run time
            return;
        }
        Sleep(universalPause);
    }
}

/*
Plays the appropriate music frequencies on beat according to a specified song array, with guards to prevent unexpected jumps or other behaviors
*/
void Robot::musicPlayer() {
    if (!musicStarted) {
        return;
    }

    long duration = millis() - musicStartTime;
    long totalLength = song_length_Megalovania;
    if (duration > totalLength) {
        if (musicLoop) {
            musicStartTime = millis();
            currentIndex = 0;
            currentFrameInIndex = 0;
            previousFrames = 0;
        } else {
            Buzzer.Off();
            musicStarted = false;
        }
    } else {
        long temp = duration / musicInterval;
        if (temp > previousFrames) {
            previousFrames = temp;
            currentFrameInIndex++;
            if (currentFrameInIndex >= pgm_read_word(&(song_Megalovania[currentIndex][1]))) {
                currentFrameInIndex = 0;
                currentIndex++;
            }
            Buzzer.Tone(pgm_read_word(&(song_Megalovania[currentIndex][0])));
        }
    }
}

/*
Debug test function with minimal implementation to drive forward 36 inches
*/
void Robot::test1() {
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    int8_t percent = SPEED;
    int8_t inches = 36; // high so any drift will be more obvious

    float counts = inches * 40.5f;

    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    long i = 0;
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts) {
        int currentLeftCounts = left_encoder.Counts();
        int currentRightCounts = right_encoder.Counts();
        if (i % 2400 == 0) {
            LCD.Clear(BLACK);
        }
        if (i % 200 == 0) {
            LCD.Write(currentLeftCounts); // apparently I can't concatenate them
            LCD.Write(" : ");
            LCD.WriteLine(currentRightCounts);
        }
        i++;
        Pause(universalPause);

        // experimental feature, needs to be tested and refined with data
        if (fabs((left_encoder.Counts() - right_encoder.Counts())) / 40.5f > 0.126) { // around 1 degree, just trust me
            float correction = (left_encoder.Counts() - right_encoder.Counts()) / 400.0f; // random value for now, needs tuning
            left_motor.SetPercent(percent * (1 + correction));
            right_motor.SetPercent(percent * (1 - correction));
        } else { // ensures it won't over correct and just wobble back and forth
            right_motor.SetPercent(percent);
            left_motor.SetPercent(percent);
        }
        if (left_encoder.Counts() == currentLeftCounts && right_encoder.Counts() == currentRightCounts) {
            if (left_encoder.Counts() > 10 && right_encoder.Counts() > 10) {
                break;
            }
        }
    }

    right_motor.Stop();
    left_motor.Stop();
}

/*
Debug test function with minimal implementation to turn right 1080 degrees
*/
void Robot::test2() {
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    int8_t percent = 35;
    int8_t direction = 1; // turn right
    uint16_t degrees = 1080; // high so any drift will be more obvious

    const float radius = 3.675; // inches
    const float pi = 3.141592653589793238462643383;

    float counts = radius * 40.5f * degrees * pi / 180.0f;

    right_motor.SetPercent(-direction * percent);
    left_motor.SetPercent(direction * percent);

    long i = 0;
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts) {
        int currentLeftCounts = left_encoder.Counts();
        int currentRightCounts = right_encoder.Counts();
        if (i % 2400 == 0) {
            LCD.Clear(BLACK);
        }
        if (i % 200 == 0) {
            LCD.Write(currentLeftCounts);
            LCD.Write(" : ");
            LCD.WriteLine(currentRightCounts);
        }
        i++;
        Pause(universalPause);

        // experimental feature, needs to be tested and refined with data
        if (fabs((left_encoder.Counts() - right_encoder.Counts())) / 40.5f > 0.126) { // around 1 degree, just trust me
            float correction = (left_encoder.Counts() - right_encoder.Counts()) / 400.0f; // random value for now, needs tuning
            left_motor.SetPercent(direction * percent * (1 + correction));
            right_motor.SetPercent(-direction * percent * (1 - correction));
        } else { // ensures it won't over correct and just wobble back and forth
            right_motor.SetPercent(-direction * percent);
            left_motor.SetPercent(direction * percent);
        }
    }

    right_motor.Stop();
    left_motor.Stop();
}

/*
Debug test function to determine accuracy of light color detection from buttons function in tasks.cpp
*/
void Robot::test3() {
    move_forward(16, 1);
    stop("Check Position");
    move_forward(0.5, 0, false, 20);
    Pause(delay);
    stop("Check Position");

    int8_t mode = 0;
    int8_t i = 0;
    while (true) {
        LCD.Clear(BLACK);
        LCD.SetFontColor(WHITE);
        for (int i = 0; i < 15; i++) { // read 15 light values
            mode += lightColor();
            move_forward((i % 2 * 2 - 1) * i / 50.0f, 2, false, 20); // moves a bit so it's not all the same value
            LCD.Write(light_sensor.Value());
            LCD.Write(" : ");
            LCD.WriteLine(lightColor());
            Pause(delay / 5);
        }

        LCD.Clear(BLACK);
        LCD.SetFontColor(WHITE);
        LCD.WriteLine("Light Color:");
        if (mode < -5) { // check threshold for confidence
            LCD.Clear(BLUE);
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("Light Color:");
            LCD.WriteLine("Blue");
            break;
        } else if (mode > 5) {
            LCD.Clear(RED);
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("Light Color:");
            LCD.WriteLine("Red");
            break;
        } else if (i < 5) { // retries 5 times if fail
            LCD.WriteLine("Inconclusive");
            LCD.WriteLine("Trying Again...");
            i++;
            move_forward((i % 2 * 2 - 1) * i / 10.0f, 2, false, 20); // little adjustment in case it's not over the light
        } else { // defaults after 5 failed attempts
            LCD.Clear(RED);
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("Light Color:");
            LCD.WriteLine("Inconclusive");
            LCD.WriteLine("Defaulting to Red");
            break;
        }
    }

    int x, y;
    LCD.WriteLine("Touch the screen to continue");
    while(!LCD.Touch(&x,&y)) {
        Pause(universalPause);
    }
    while(LCD.Touch(&x,&y)) {
        Pause(universalPause);
    }
}

/*
Clean debug testing interface
*/
void Robot::debugTest(uint8_t test) {
    if (!debugMode) {
        return;
    }

    int x, y;
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);
    LCD.Write("Debug Test ");
    LCD.WriteLine(test);
    LCD.WriteLine("Touch the screen to begin");
    while(!LCD.Touch(&x,&y)) {
        Pause(universalPause);
    }
    while(LCD.Touch(&x,&y)) {
        Pause(universalPause);
    }
    
    if (test == 1) {
        test1();
    } else if (test == 2) {
        test2();
    } else if (test == 3) {
        test3();
    }

    LCD.Clear(GREEN);
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Test Complete");
    LCD.WriteLine("Touch the screen to continue");
    while(!LCD.Touch(&x,&y)) {
        Pause(universalPause);
    }
    while(LCD.Touch(&x,&y)) {
        Pause(universalPause);
    }
}