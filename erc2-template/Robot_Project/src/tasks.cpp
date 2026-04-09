#include <tasks.h>

void compost(Robot &robot) {
    robot.rotate(2, -90);
    Sleep(delay);
    robot.rotate(0, -90, true, 1, -20);
    Sleep(delay);
    robot.rotate(2, 20);
    Sleep(delay);
    robot.move_forward(2, 2);
    Sleep(delay);
    robot.rotate(1, 20, true, 2, -20);
    robot.stop();
    Sleep(delay);
    robot.rotate(0, 90, true);
    Sleep(delay);
    robot.rotate(0, -90, true);
    robot.stop();
    Sleep(delay);
    robot.rotate(1, -30, true, 2, 30);
    Sleep(delay);
    robot.turn(15, 1);
    Sleep(delay);
    robot.rotate(0, 0, true, 1, 0);
    Sleep(delay);
    robot.rotate(2, 5, true);
    Sleep(delay);
    robot.turn(20, 1);
    robot.defaultArm();
    Sleep(delay);
    robot.turn(35, -1);
}

void lift(Robot &robot) {
    robot.rotate(1, 20, false, 2, -32);
    Sleep(delay);
    robot.move_forward(4);
    Sleep(delay);
    robot.rotate(2, -10, true);
    Sleep(delay);
    robot.rotate(2, -40, true, 1, 90);
}

void drop(Robot &robot) {
    robot.rotate(0, 20, true, 2, -70);
    Sleep(1500);
    robot.move_forward(-2.5, 2);
    robot.defaultArm();
}

void levers(Robot &robot) {
    int8_t lever = robot.lever();
    float distBetweenLevers = 5;
    if (lever > 0) {
        robot.turn(90, -1);
        Sleep(delay);
        robot.move_forward(lever * distBetweenLevers, 2);
        Sleep(delay);
        robot.turn(90, 1);
        Sleep(delay);
    }
    lever += -1;
    robot.rotate(1, -20, true);
    robot.move_forward(-2, 2);
    Sleep(delay);
    robot.rotate(1, -60);
    robot.move_forward(2, 2);
    Sleep(4500);
    robot.rotate(1, -5, true);
    Sleep(delay);
    robot.move_forward(-4, 2);
    robot.defaultArm();
    if (lever != 0) {
        robot.turn(90, lever);
        Sleep(delay);
        robot.move_forward(distBetweenLevers, 2);
        Sleep(delay);
        robot.turn(lever * 45 + 90, lever);
    } else {
        robot.turn(135, -1);
    }
}

void buttons(Robot &robot) {
    robot.move_forward(-0.5);
    Sleep(500);

    int8_t mode = 0;
    int8_t i = 0;
    while (true) {
        for (int i = 0; i < 15; i++) {
            mode += robot.lightColor();
            Sleep(delay / 5);
        }

        LCD.Clear(BLACK);
        LCD.SetFontColor(WHITE);
        LCD.WriteLine("Light Color:");
        if (mode < -5) {
            LCD.Clear(BLACK);
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("Light Color:");
            LCD.WriteLine("Blue");
            mode = -1;
            break;
        } else if (mode > 5) {
            LCD.Clear(BLACK);
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("Light Color:");
            LCD.WriteLine("Red");
            mode = 1;
            break;
        } else if (i < 5) {
            LCD.WriteLine("Inconclusive");
            LCD.WriteLine("Trying Again...");
            mode = 0;
            i++;
        } else {
            LCD.Clear(BLACK);
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("Light Color:");
            LCD.WriteLine("Inconclusive");
            LCD.WriteLine("Defaulting to Red");
            mode = 1;
        }
    }

    robot.rotate(0, -mode * 45);
    Sleep(delay);
    robot.rotate(1, -40, true, 2, 40);
    Sleep(delay);
    robot.rotate(1, -30, true, 2, 30);
    Sleep(delay);
    robot.rotate(1, -40, true, 2, 40);
    Sleep(delay);
    robot.defaultArm();
    robot.turn(180, 1);
}

void window(Robot &robot) {
    robot.rotate(0, -40, true, 2, -90);
    Sleep(delay);
    robot.rotate(0, 40, true, 2, -70);
    Sleep(delay);
    robot.rotate(0, 20, true, 2, 90);
    Sleep(delay);
    robot.rotate(0, 50, true, 2, -40);
    Sleep(delay);
    robot.rotate(0, -10, true, 2, -90);
    Sleep(delay);
    robot.move_forward(-4, 2);
    Sleep(delay);
    robot.defaultArm();
}

void dance(Robot &robot) {
    for (int i = 0; i < 10; i++) {
        robot.move_forward(1);
        robot.move_forward(-1);
        robot.move_forward(1);
        robot.move_forward(-1);
        robot.turn(60, 1);
    }
}

void runCourse(Robot &robot) { // god function hahahaha bad practice can't stop me
    robot.initialize();
    
    // start button
    robot.move_forward(5, 2);
    Sleep(delay);
    robot.turn(135, -1);
    Sleep(delay);

    // initial alignment step so starting position doesn't matter
    robot.move_forward(3, 2);
    Sleep(delay);
    robot.turn(90, -1);
    Sleep(delay);
    robot.move_forward(-20, 2);
    Sleep(delay);
    robot.move_forward(8, 2);
    Sleep(delay);
    robot.turn(90, 1);
    Sleep(delay);
    robot.move_forward(-20, 2);
    Sleep(delay);

    // compost
    robot.move_forward(2, 2);
    Sleep(delay);
    robot.turn(90, -1);
    Sleep(delay);
    robot.move_forward(-2.5, 2);
    Sleep(delay);
    robot.stop(); // checkpoint
    compost(robot);
    Sleep(delay);

    // window
    robot.move_forward(3, 2);
    Sleep(delay);
    robot.turn(90, 1);
    Sleep(delay);
    robot.move_forward(-5, 2);
    Sleep(delay);
    robot.move_forward(36, 2);
    Sleep(delay);
    robot.move_forward(-2.5, 2);
    Sleep(delay);
    robot.stop(); // checkpoint
    window(robot);
    Sleep(delay);

    // lift bucket
    robot.move_forward(12, 2);
    Sleep(delay);
    robot.move_forward(-4);
    Sleep(delay);
    robot.turn(90, -1);
    robot.stop(); // checkpoint
    Sleep(delay);
    robot.move_forward(3.5, 2);
    Sleep(delay);
    lift(robot);
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
    robot.move_forward(1, 2);
    Sleep(delay);
    robot.turn(90, -1);
    Sleep(delay);
    robot.move_forward(26, 2);
    Sleep(delay);
    robot.hug(1);
    Sleep(delay);
    robot.turn(5, 1);
    Sleep(delay);
    robot.move_forward(24, 2);
    Sleep(delay);
    robot.move_forward(2, 2);
    Sleep(delay);
    robot.stop(); // checkpoint
    drop(robot);
    Sleep(delay);

    // levers
    robot.turn(90, -1);
    Sleep(delay);
    robot.move_forward(-9, 2);
    Sleep(delay);
    robot.move_forward(14, 2);
    Sleep(delay);
    robot.turn(90, 1);
    robot.stop(); // checkpoint
    Sleep(delay);
    robot.move_forward(16, 2);
    Sleep(delay);
    robot.turn(90, -1);
    Sleep(delay);
    robot.move_forward(2.5, 2);
    Sleep(delay);
    robot.turn(45, 1);
    Sleep(delay);
    robot.move_forward(-1, 2);
    Sleep(delay);
    robot.stop(); // checkpoint
    levers(robot);
    Sleep(delay);
    robot.stop(); // checkpoint

    // buttons
    robot.move_forward(16, 1); // might add realignment step between if needed
    robot.stop(); // checkpoint
    Sleep(delay);
    robot.move_forward(3, 2);
    Sleep(delay);
    robot.turn(90, 1);
    Sleep(delay);
    robot.stop(); // checkpoint
    robot.move_forward(-6, 1);
    Sleep(delay);
    robot.stop(); // checkpoint
    buttons(robot);
    Sleep(delay);

    // stop button
    robot.move_forward(34, 2);
    Sleep(delay);
    robot.move_forward(-0.6);
    Sleep(delay);
    robot.turn(90, 1);
    Sleep(delay);
    robot.move_forward(-6, 2);
    Sleep(delay);
    robot.move_forward(33, 2);
    Sleep(delay);
    robot.turn(90, 1);
    Sleep(delay);
    robot.move_forward(-10, 2);
    Sleep(delay);
    robot.move_forward(10, 2);
    Sleep(delay);
    robot.turn(135, -1);
    Sleep(delay);
    robot.move_forward(10, 2);
    Sleep(delay);
    robot.stop(); // checkpoint
    
    // victory dance
    robot.move_forward(-4);
    Sleep(delay);
    dance(robot);
}