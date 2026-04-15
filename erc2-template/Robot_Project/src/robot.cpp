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

uint8_t clampServo(float value) { // single function, not worth using the utils file
    if (value > 180) {
        return 180;
    } else if (value < 0) {
        return 0;
    } else {
        return value;
    }
}

void Robot::initialize() {
    defaultArm();
    RCS.InitializeTouchMenu("0300G9LQG");

    WaitForFinalAction();
    
    // int x, y;
    // LCD.Clear(BLACK);
    // LCD.SetFontColor(WHITE);
    // LCD.WriteLine("Paused");
    // LCD.WriteLine("Touch the screen to continue");
    // while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    // while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // LCD.WriteLine("3");
    // Sleep(1000);
    // LCD.WriteLine("2");
    // Sleep(1000);
    // LCD.WriteLine("1");
    // Sleep(1000);
    
    while (!detect(1));
    musicStartTime = millis();
}

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

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts && !detect(early)) {
        int currentLeftCounts = left_encoder.Counts();
        int currentRightCounts = right_encoder.Counts();
        Pause(universalPause);
        if (early == 2) {
            if (left_encoder.Counts() == currentLeftCounts && right_encoder.Counts() == currentRightCounts) {
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
        }
    }
    if (detect(early)) {
        stopped = true;
    }

    right_motor.Stop();
    left_motor.Stop();
    return stopped;
}

bool Robot::turn(int16_t degrees, int8_t direction, int8_t early) { // positive: turn right, negative: turn left
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    int8_t percent = SPEED;
    float adjustment = 1.0f;

    const float radius = 3.6; // inches
    const float pi = 3.141592653589793238462643383;

    float counts = radius * 40.5f * degrees * pi / 180.0f;

    right_motor.SetPercent(-direction * percent);
    left_motor.SetPercent(direction * percent);

    bool stopped = false;

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts * adjustment && !detect(early)) {
        Pause(universalPause);
    }
    if (detect(early)) {
        stopped = true;
    }

    right_motor.Stop();
    left_motor.Stop();
    return stopped;
}

bool Robot::detect(int8_t type) { // 0 = none, 1 = light
    if (type == 0) {
        return false;
    }
    bool detected = false;
    if (type == 1) {
        if (lightColor() != 0) {
            detected = true;
        }
    }
    return detected;
}

int8_t Robot::lightColor() { // 0 = no light, -1 = blue, 1 = red
    if (light_sensor.Value() < 0.8) { // && light_sensor.Value() > 0.6) {
        return 1;
    } else if (light_sensor.Value() < 3 && light_sensor.Value() > 0.8) {
        return -1;
    } else {
        return 0;
    }
}

void Robot::stop() {
    if (debugMode) {
        int x, y;
        LCD.Clear(BLACK);
        LCD.SetFontColor(WHITE);
        LCD.WriteLine("Paused");
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
}

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
Retired function
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

void Robot::rotate(int8_t joint, int16_t angle, boolean slow, int8_t joint2, int16_t angle2) {
    int16_t moveAngle = angle;
    int16_t moveAngle2 = angle2;
    if (joint == 0) {
        moveAngle = angle + 90;
    } else if (joint == 1) {
        moveAngle = angle + 90;
    } else if (joint == 2) {
        moveAngle = 100 - angle;
    }

    if (joint2 == 0) {
        moveAngle2 = angle2 + 90;
    } else if (joint2 == 1) {
        moveAngle2 = angle2 + 90;
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
        servos[joint].SetDegree(clampServo(angles[joint] + increment * (i + 1)));

        if (joint2 != 4) {
            servos[joint2].SetDegree(clampServo(angles[joint2] + increment2 * (i + 1)));
        }
        Pause(rotateSpeed);
    }
    angles[joint] = moveAngle;
    if (joint2 != 4) {
        angles[joint2] = moveAngle2;
    }
}

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
            moveAngle[i] = angle[i] + 90;
        } else if (joint[i] == 2) {
            moveAngle[i] = 100 - angle[i];
        }
        increment[i] = 1.0f * (moveAngle[i] - angles[joint[i]]) / steps;
    }

    for (int i = 0; i < steps; i++) {
        for (int j = 0; j < size; j++) {
            servos[joint[j]].SetDegree(clampServo(angles[joint[j]] + increment[j] * (i + 1)));
        }
        Pause(rotateSpeed);
    }
    for (int i = 0; i < size; i++) {
        angles[joint[i]] = moveAngle[i];
    }
}

void Robot::defaultArm() {
    // rotate(0, 0);
    // rotate(1, 90);
    // rotate(2, 90);
    servos[0].SetDegree(90);
    servos[1].SetDegree(180);
    servos[2].SetDegree(10);
    angles[0] = 90;
    angles[1] = 180;
    angles[2] = 10;
}

int8_t Robot::lever() {
    return 2 - RCS.GetLever();
}

void Robot::sprint(float inches, int8_t highSpeed, int8_t lowSpeed) {
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
        right_motor.SetPercent(adjustment * percent);
        left_motor.SetPercent(percent);

        Pause(universalPause); // emergency brakes, should never be triggered, just a backup
        if (left_encoder.Counts() == currentLeftCounts && right_encoder.Counts() == currentRightCounts) {
            break;
        }
    }

    right_motor.Stop();
    left_motor.Stop();
}

void Robot::Pause(uint16_t milli) {
    int increments = milli / universalPause;
    for (int i = 0; i < increments; i++) {
        musicPlayer();
        Sleep(universalPause);
    }
}

void Robot::musicPlayer() {
    long duration = millis() - musicStartTime;
    long totalLength = song_length_Asgore;
    if (duration > totalLength) {
        if (musicLoop) {
            musicStartTime = millis();
            currentIndex = 0;
            currentFrameInIndex = 0;
            previousFrames = 0;
        } else {
            Buzzer.Off();
        }
    } else {
        long temp = duration / musicInterval;
        if (temp > previousFrames) {
            previousFrames = temp;
            currentFrameInIndex++;
            if (currentFrameInIndex >= pgm_read_word(&(song_Asgore[currentIndex][1]))) {
                currentFrameInIndex = 0;
                currentIndex++;
            }
            Buzzer.Tone(pgm_read_word(&(song_Asgore[currentIndex][0])));
        }
    }
}