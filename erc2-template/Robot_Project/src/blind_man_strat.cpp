#include <blind_man_strat.h>

// motors
FEHMotor left_motor(FEHMotor::Motor1, 9);
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

bool Robot::move_forward(float inches, int8_t early) {
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    int8_t percent = SPEED;

    float counts = inches * 40.5f;

    if (counts < 0) {
        counts *= -1;
        percent = -SPEED;
    }

    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    bool stopped = false;

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts && !detect(early)) {
        if (early == 2) {
            int currentLeftCounts = left_encoder.Counts();
            int currentRightCounts = right_encoder.Counts();
            Sleep(check_time);
            if (left_encoder.Counts() == currentLeftCounts && right_encoder.Counts() == currentRightCounts) {
                stopped = true;
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

    const float radius = 3.35; // inches
    const float pi = 3.141592653589793238462643383;

    float counts = radius * 40.5f * degrees * pi / 180.0f;

    right_motor.SetPercent(-direction * percent);
    left_motor.SetPercent(direction * percent);

    bool stopped = false;

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts && !detect(early));
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

void Robot::hug(int8_t side) { // 0 = front, 1 = right side, -1 = left side
    turn(90, -side);
    Sleep(delay);
    move_forward(-8, 2);
    Sleep(delay);
    turn(90, side);
}

void Robot::sweep(float inches, int8_t angle, float gap) {
    turn(angle / 2, 1, 1);
    for (int i = 0; i < inches / gap; i++) {
        move_forward(gap, 1);
        bool found = turn(angle, (i % 2) * 2 - 1, 1);
        if (found) {
            break;
        }
    }
}

void hugTheWall(Robot robot, int8_t inches, int8_t side, int8_t checks, int8_t early) {
    float interval = inches / (checks + 1.0f);
    int8_t delay = 100;

    for (int i = 0; i < checks; i++) {
        robot.move_forward(interval, early);
        Sleep(delay);
        robot.hug(side);
        Sleep(delay);
    }

    robot.move_forward(interval, early);
}

void buttons(Robot robot) {
    int8_t mode = robot.lightColor();
    int8_t delay = 100;

    robot.turn(90, mode);
    Sleep(delay);
    robot.move_forward(3);
    Sleep(delay);
    robot.turn(90, -mode);
    Sleep(delay);
    robot.move_forward(8, 2);
    Sleep(delay);
    robot.move_forward(-3);
    Sleep(delay);
    robot.turn(90, -mode);
    Sleep(delay);
    robot.move_forward(3);
    Sleep(delay);
    robot.turn(90, -mode);
}