#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <controls.h>

void ERCMain()
{
    Robot robot;
    robot.stop();

    robot.tempMove(10.0f, 1);
    robot.stop();
    robot.tempMove(10.0f, 1);
    Sleep(100);
    robot.tempMove(10.0f, -1);
    // navigate();
}