#include <robot.h>

// motors
FEHMotor left_motor(FEHMotor::Motor2, 9);
FEHMotor right_motor(FEHMotor::Motor0, 9);

// sensors
DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin9);
AnalogInputPin light_sensor(FEHIO::Pin3);

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
    // RCS.InitializeTouchMenu("0300G9LQG");
    
    int x, y;
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Paused");
    LCD.WriteLine("Touch the screen to continue");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    LCD.WriteLine("3");
    Sleep(1000);
    LCD.WriteLine("2");
    Sleep(1000);
    LCD.WriteLine("1");
    Sleep(1000);
    
    while (!detect(1));
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
        if (early == 2) {
            int currentLeftCounts = left_encoder.Counts();
            int currentRightCounts = right_encoder.Counts();
            Sleep(check_time);
            if (left_encoder.Counts() == currentLeftCounts && right_encoder.Counts() == currentRightCounts) {
                stopped = true;
                if (!backUp) { // default false since slipping causes inconsistencies
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

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts * adjustment && !detect(early));
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
    if (light_sensor.Value() < red_threshold) {
        return 1;
    } else if (light_sensor.Value() < blue_threshold) {
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
        while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
        while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
        LCD.WriteLine("3");
        Sleep(1000);
        LCD.WriteLine("2");
        Sleep(1000);
        LCD.WriteLine("1");
        Sleep(1000);
    }
}

void Robot::hug(int8_t side) { // 0 = front, 1 = right side, -1 = left side
    turn(90, -side);
    Sleep(delay);
    move_forward(-12, 2);
    Sleep(delay);
    move_forward(1);
    Sleep(delay);
    turn(90, side);
}

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
        Sleep(rotateSpeed);
    }
    angles[joint] = moveAngle;
    if (joint2 != 4) {
        angles[joint2] = moveAngle2;
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