#include <controls.h>

FEHMotor left_motor(FEHMotor::Motor1, 9);
FEHMotor right_motor(FEHMotor::Motor0, 9);
DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin9);

void driveTrain::move_forward(int percent, int inches) // I'm gonna steal this function and change it to take float inches
{
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

void driveTrain::turn(int percent, int degrees, int direction) // positive: turn right, negative: turn left
{
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