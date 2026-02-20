#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHSD.h>
#include <FEH.h>

// Declarations for analog optosensors
AnalogInputPin right_opto(FEHIO::Pin8);
AnalogInputPin middle_opto(FEHIO::Pin9);
AnalogInputPin left_opto(FEHIO::Pin10);

#define LEFT_SPEED 25
#define RIGHT_SPEED 25

void ERCMain()
{
    FEHMotor left(FEHMotor::Motor0, 9);
    FEHMotor right(FEHMotor::Motor1, 9);

    left.SetPercent(LEFT_SPEED);
    right.SetPercent(RIGHT_SPEED);
    while (true) {
        if (left_opto.Value() > 3) {
            left.SetPercent(LEFT_SPEED * 1.4);
            right.SetPercent(RIGHT_SPEED * 0.2);
        } else if (right_opto.Value() > 2.5) {
            left.SetPercent(LEFT_SPEED * 0.2);
            right.SetPercent(RIGHT_SPEED * 1.4);
        } else {
            left.SetPercent(LEFT_SPEED);
            right.SetPercent(RIGHT_SPEED);
        }
    }
}
