#include <blind_man_strat.h>

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

bool Robot::move_forward(float inches, int8_t early, int8_t speed) {
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    int8_t percent = speed;
    float adjustment = 1.03;

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
    float adjustment = 1.05;

    const float radius = 3.35; // inches
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

void compost(Robot robot) {
    LCD.WriteLine("compost");
    robot.stop();
}

void lift(Robot robot) {
    LCD.WriteLine("lift");
    robot.stop();
}

void drop(Robot robot) {
    LCD.WriteLine("drop");
    robot.stop();
}

void levers(Robot robot) {
    LCD.WriteLine("levers");
    robot.stop();
}

void buttons(Robot robot) {
    robot.move_forward(-0.5);
    Sleep(500);

    int8_t mode = 0;
    while (true) {
        for (int i = 0; i < 15; i++) {
            mode += robot.lightColor();
            Sleep(delay / 5);
        }

        LCD.WriteLine("Light Color:");
        if (mode < -5) {
            LCD.WriteLine("Blue");
            mode = -1;
            break;
        } else if (mode > 5) {
            LCD.WriteLine("Red");
            mode = 1;
            break;
        } else {
            LCD.WriteLine("Inconclusive");
            LCD.WriteLine("Trying Again...");
            mode = 0;
        }
    }

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

void window(Robot robot) {
    LCD.WriteLine("window");
    robot.stop();
}

void dance(Robot robot) {
    for (int i = 0; i < 10; i++) {
        robot.move_forward(1);
        robot.move_forward(-1);
        robot.move_forward(1);
        robot.move_forward(-1);
        robot.turn(60, 1);
    }
}

void runCourse(Robot robot) { // god function hahahaha bad practice can't stop me
    while (!robot.detect(1));
    
    // start button
    robot.move_forward(5, 2);
    Sleep(delay);
    robot.turn(135, 1);
    Sleep(delay);

    // compost
    robot.move_forward(5, 2);
    Sleep(delay);
    robot.hug(-1);
    Sleep(delay);
    robot.stop(); // checkpoint
    robot.move_forward(20, 2);
    Sleep(delay);
    compost(robot);
    Sleep(delay);

    // lift bucket
    robot.turn(90, 1);
    Sleep(delay);
    robot.move_forward(36, 2);
    Sleep(delay);
    robot.move_forward(-6);
    Sleep(delay);
    robot.turn(90, -1);
    robot.stop(); // checkpoint
    Sleep(delay);
    robot.move_forward(2, 2);
    Sleep(delay);
    lift(robot);
    robot.move_forward(-4);
    Sleep(delay);

    // drop bucket
    robot.turn(90, -1);
    Sleep(delay);
    robot.move_forward(5, 2);
    Sleep(delay);
    robot.turn(90, -1);
    robot.stop(); // checkpoint
    Sleep(delay);
    robot.move_forward(36, 2);
    Sleep(delay);
    robot.turn(90, -1);
    Sleep(delay);
    robot.move_forward(40, 2);
    Sleep(delay);
    robot.turn(90, -1);
    Sleep(delay);
    robot.move_forward(-9, 2);
    Sleep(delay);
    robot.move_forward(5, 2);
    Sleep(delay);
    robot.turn(90, 1);
    robot.stop(); // checkpoint
    Sleep(delay);
    robot.move_forward(16, 2);
    Sleep(delay);
    robot.turn(90, 1);
    Sleep(delay);
    drop(robot);
    Sleep(delay);
    robot.turn(90, -1);
    Sleep(delay);

    // levers
    robot.move_forward(20, 2);
    Sleep(delay);
    robot.move_forward(-2, 2);
    Sleep(delay);
    robot.turn(90, -1);
    robot.stop(); // checkpoint
    Sleep(delay);
    robot.move_forward(10, 2);
    Sleep(delay);
    robot.turn(45, 1);
    Sleep(delay);
    levers(robot);
    Sleep(delay);
    robot.turn(135, -1);
    robot.stop(); // checkpoint
    Sleep(delay);

    // buttons
    robot.move_forward(36, 2);
    robot.stop(); // checkpoint
    Sleep(delay);
    robot.move_forward(-6, 2);
    Sleep(delay);
    robot.turn(90, 1);
    Sleep(delay);
    robot.stop(); // checkpoint
    robot.move_forward(-5, 1);
    Sleep(delay);
    robot.stop(); // checkpoint
    buttons(robot);
    Sleep(delay);

    // stop button
    robot.move_forward(34, 2);
    Sleep(delay);
    robot.move_forward(0.6);
    Sleep(delay);
    robot.turn(90, 1);
    Sleep(delay);
    robot.move_forward(35.5, 2);
    Sleep(delay);
    robot.turn(45, -1);
    Sleep(delay);
    robot.move_forward(4, 2);
    robot.stop(); // checkpoint
    
    // victory dance
    Sleep(delay);
    robot.move_forward(-4);
    Sleep(delay);
    dance(robot);
}