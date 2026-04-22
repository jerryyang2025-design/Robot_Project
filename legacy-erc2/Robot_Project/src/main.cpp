#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <controls.h>

void ERCMain()
{
    Robot robot;
    robot.pause();

    robot.controlledFollow(10, 1);
    robot.turn(90, 1);
    robot.pause();
    // navigate();
}