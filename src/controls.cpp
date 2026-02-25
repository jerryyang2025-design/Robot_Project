#include <controls.h>

// motors
FEHMotor left_motor(FEHMotor::Motor1, 9);
FEHMotor right_motor(FEHMotor::Motor0, 9);

// sensors
DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin9);
AnalogInputPin left_opto(FEHIO::Pin0);
AnalogInputPin right_opto(FEHIO::Pin1);

// servos
FEHServo base(FEHServo::Servo0);
FEHServo joint1(FEHServo::Servo1);
FEHServo joint2(FEHServo::Servo2);

void Robot::move_forward(int inches, int percent, int early) {
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    float counts = inches * 40.5f;

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void Robot::turn(int degrees, int direction, int percent) { // positive: turn right, negative: turn left
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    const float radius = 3.5; // inches
    const float pi = 3.141592653589793238462643383;

    float counts = radius * 40.5f * degrees * pi / 180.0f;

    //Set both motors to desired percent
    right_motor.SetPercent(-direction * percent);
    left_motor.SetPercent(direction * percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void Robot::controlledFollow(int inches, int direction, int early) {
    float counts = inches * 40.5f;
    while (!detect(early) && (left_encoder.Counts() + right_encoder.Counts()) / 2. < counts) {
        if (left_opto.Value() > left_opto_threshold) {
            left_motor.SetPercent(SPEED * 0.6);
            right_motor.SetPercent(SPEED * 1.3);
            left_motor.SetPercent(SPEED * 1.4);
            right_motor.SetPercent(SPEED * 0.2);
        } else if (right_opto.Value() > right_opto_threshold) {
            left_motor.SetPercent(SPEED * 1.3);
            right_motor.SetPercent(SPEED * 0.6);
            left_motor.SetPercent(SPEED * 0.2);
            right_motor.SetPercent(SPEED * 1.4);
        } else {
            left_motor.SetPercent(SPEED);
            right_motor.SetPercent(SPEED);
        }
    }
}

void Robot::rotate(int joint, int angle) {
    FEHServo servos[] = {base, joint1, joint2};
    servos[joint].SetDegree(angle);
}

void Robot::defaultArm() {
    for (int i = 0; i < 3; i++) {
        rotate(i, 0);
    }
}