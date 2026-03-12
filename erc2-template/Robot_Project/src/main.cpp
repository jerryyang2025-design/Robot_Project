#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <blind_man_strat.h>
#include <tasks.h>

void ERCMain()
{
    Robot robot;
    robot.stop();

    int8_t pause = 100;

    while (!robot.detect(1));
    
    robot.move_forward(-3);
    Sleep(pause);
    robot.turn(45, 1);
    Sleep(pause);
    hugTheWall(robot, 42, 1, 3, 2);
    Sleep(pause);
    robot.turn(90, -1);
    Sleep(pause);
    robot.move_forward(-6, 2);
    Sleep(pause);
    hugTheWall(robot, 24, -1, 2, 2);
    Sleep(pause);
    robot.sweep();
    Sleep(pause);
    buttons(robot);
    Sleep(pause);
    hugTheWall(robot, 34, 1, 2, 2);
    Sleep(pause);
    robot.turn(90, 1);
    Sleep(pause);
    robot.move_forward(-6, 2);
    Sleep(pause);
    hugTheWall(robot, 40, -1, 1, 2);
}