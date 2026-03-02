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
FEHServo servos[3] = {base, joint1, joint2};

void Robot::move_forward(int8_t inches, int8_t percent, int8_t early) {
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

void Robot::turn(int16_t degrees, int8_t direction, int8_t percent) { // positive: turn right, negative: turn left
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

void Robot::follow(FEHMotor motor1, FEHMotor motor2) {
    motor1.SetPercent(SPEED * 1.3);
    motor2.SetPercent(SPEED * 0.6);
    motor1.SetPercent(SPEED * 0.2);
    motor2.SetPercent(SPEED * 1.4); // why did I have 4 lines here again? should probably test and fix it...
}

bool Robot::detect(int8_t type) {
    if (type == 0) {
        return false;   
    }
    bool detected = false;
    if (type == 1) {
        // TODO: finish this
    } else if ( type == 2) {
        
    } else if ( type == 3) {
        
    }
    return detected;
}

void Robot::controlledFollow(int8_t inches, int8_t direction, int8_t early) {
    float counts = inches * 40.5f;
    while (!detect(early) && (left_encoder.Counts() + right_encoder.Counts()) / 2. < counts) {
        bool left_detected = left_opto.Value() > left_opto_threshold;
        bool right_detected = right_opto.Value() > right_opto_threshold;
        bool both = right_detected && left_detected;
        if (direction > 0 && both) {
            follow(left_motor, right_motor);
            continue;
        }
        if (direction < 0 && both) {
            follow(right_motor, left_motor);
            continue;
        }

        if (left_detected) {
            follow(right_motor, left_motor);
        } else if (right_detected) {
            follow(left_motor, right_motor);
        } else {
            left_motor.SetPercent(SPEED);
            right_motor.SetPercent(SPEED);
        }
    }
}

void Robot::rotate(int8_t joint, int16_t angle) {
    int increment = (angle - angles[joint]) / rotateIncrement;
    for (int i = 0; i < rotateIncrement; i++) {
        servos[joint].SetDegree(angles[joint] + increment * (i + 1));
    }
    angles[joint] = angle;
}

void Robot::defaultArm() {
    for (int i = 0; i < 3; i++) {
        rotate(i, 0);
    }
}