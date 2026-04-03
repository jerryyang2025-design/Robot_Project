#include <tasks.h>

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