#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <blind_man_strat.h>
#include <tasks.h>

AnalogInputPin test(FEHIO::Pin3);

void ERCMain()
{
    Robot robot;
    robot.stop();

    int8_t pause = 100;

    while (!robot.detect(1));
    
    robot.move_forward(5, 2);
    Sleep(pause);
    robot.turn(135, -1);
    Sleep(pause);

    // hugTheWall(robot, 41, 1, 1, 2);
    robot.move_forward(5, 2);
    Sleep(pause);
    robot.hug(1);
    Sleep(pause);
    robot.move_forward(1);
    Sleep(pause);
    // robot.stop();
    robot.move_forward(42, 2);
    Sleep(pause);

    robot.turn(90, -1);
    Sleep(pause);
    robot.move_forward(-9, 2);
    Sleep(pause);
    // robot.stop();

    // hugTheWall(robot, 24, -1, 1, 2);
    robot.move_forward(16, 1);
    Sleep(pause);

    robot.sweep();
    Sleep(pause);
    // robot.stop();

    buttons(robot);
    Sleep(pause);
    // robot.stop();

    // hugTheWall(robot, 34, 1, 1, 2);
    robot.move_forward(34, 2);
    Sleep(pause);

    robot.move_forward(0.6);
    Sleep(pause);
    robot.turn(90, 1);
    Sleep(pause);
    // robot.stop();

    // hugTheWall(robot, 40, -1, 1, 2);
    robot.move_forward(35.5, 2);
    Sleep(pause);
    robot.turn(45, -1);
    Sleep(pause);
    robot.move_forward(4);
    
    Sleep(pause);
    robot.move_forward(-4);
    Sleep(pause);
    robot.turn(1000, 1);
}