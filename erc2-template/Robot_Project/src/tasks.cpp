#include <tasks.h>

void compost(Robot &robot) {
    robot.rotate(2, -90);
    robot.rotate(0, -90, true, 1, -20);
    // robot.rotate((int8_t[]){0, 1, 2}, (int16_t[]){-90, -20, -90}, 3, true);
    robot.rotate(2, 20); // finishes positioning
    robot.move_forward(0.8, 2); // moves the arm under the compost
    robot.rotate(1, 20, true, 2, -20); // rotate 90
    robot.stop(); // debug checkpoint
    robot.rotate(0, 90, true); // rotate 180
    robot.Pause(delay);
    robot.rotate(0, -90, true); // rotate back 180
    robot.stop(); // debug checkpoint
    robot.rotate(1, -30, true, 2, 30); // rotate back 45
    robot.move_forward(-2, 2); // clearance
    robot.rotate(0, 70, true);
    robot.rotate(1, -20, true, 2, 20);
    robot.move_forward(2, 2); // positioning
    robot.stop(); // debug checkpoint
    robot.rotate(0, 5, true); // finish rotating
    robot.Pause(delay);
    robot.move_forward(-2, 2); // clearance
    robot.defaultArm();
    robot.move_forward(2, 2); // move back
}

void lift(Robot &robot) {
    robot.rotate(1, 20, false, 2, -32); // positioning
    robot.move_forward(4);
    robot.rotate(2, -10, true);
    robot.rotate(2, -40, true, 1, 90); // lift
}

void drop(Robot &robot) {
    robot.rotate(0, 23, true, 2, -90); // drop
    robot.Pause(1000);
    robot.move_forward(-1.5, 2);
    robot.defaultArm();
    robot.move_forward(0.7, 2);
}

void levers(Robot &robot) {
    int8_t lever = robot.lever();
    const float distBetweenLevers = 4;
    if (lever > 0) { // move to correct lever start from right
        robot.turn(90, -1);
        robot.Pause(delay);
        robot.move_forward(lever * distBetweenLevers, 2);
        robot.Pause(delay);
        robot.turn(90, 1);
        robot.Pause(delay);
    }
    lever += -1;
    robot.rotate(1, -20, true); // push lever down
    robot.move_forward(-2, 2);
    robot.Pause(delay);
    robot.rotate(1, -60);
    robot.move_forward(2, 2); // move arm under lever
    robot.Pause(4500);
    robot.rotate(1, -5, true); // push lever back up
    robot.Pause(delay);
    robot.move_forward(-4, 2);
    robot.defaultArm();
    /*
    Reworked to fit the new order, kept in case it fails
    */
    // if (lever != 0) { // move to middle lever and turn to buttons
    //     robot.turn(90, lever);
    //     robot.Pause(delay);
    //     robot.move_forward(distBetweenLevers, 2);
    //     robot.Pause(delay);
    //     robot.turn(lever * 45 + 90, lever);
    // } else {
    //     robot.turn(135, -1);
    // }
    if (lever != 0) { // move to middle lever
        robot.turn(90, lever);
        robot.Pause(delay);
        robot.move_forward(distBetweenLevers, 2);
        robot.Pause(delay);
        robot.turn(90, -lever);
    }
}

void buttons(Robot &robot) {
    robot.move_forward(0.5, 0, false, 20);
    robot.Pause(500);

    int8_t mode = 0;
    int8_t i = 0;
    while (true) {
        for (int i = 0; i < 15; i++) { // read 15 light values
            mode += robot.lightColor();
            robot.Pause(delay / 5);
        }

        robot.Pause(delay / 5);
        LCD.Clear(BLACK);
        LCD.SetFontColor(WHITE);
        LCD.WriteLine("Light Color:");
        if (mode < -5) { // check threshold for confidence
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
        } else if (i < 5) { // retries 5 times if fail
            LCD.WriteLine("Inconclusive");
            LCD.WriteLine("Trying Again...");
            mode = 0;
            i++;
            robot.move_forward((i % 2 * 2 - 1) * i / 10.0f, 2, false, 20); // little adjustment in case it's not over the light
        } else { // defaults after 5 failed attempts
            LCD.Clear(BLACK);
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("Light Color:");
            LCD.WriteLine("Inconclusive");
            LCD.WriteLine("Defaulting to Red");
            mode = 1;
            break;
        }
    }

    robot.turn(90, mode);
    robot.Pause(delay);
    robot.move_forward(3);
    robot.Pause(delay);
    robot.turn(90, -mode);
    robot.Pause(delay);
    robot.move_forward(8, 2);
    robot.Pause(delay);
    robot.move_forward(-3);
    robot.Pause(delay);
    robot.turn(90, -mode);
    robot.Pause(delay);
    robot.move_forward(3);
    robot.Pause(delay);
    robot.turn(90, -mode);
}

void window(Robot &robot) {
    robot.rotate(0, -50);
    robot.rotate(1, 70, true, 2, -90); // positioning
    robot.rotate(0, 30, true); // open window
    robot.Pause(delay);
    robot.rotate(1, 90, true, 2, 0); // clearance
    robot.rotate(0, 50, true);
    robot.rotate(1, 70, true, 2, -90); // positioning
    robot.rotate(0, -30, true); // close window
    robot.Pause(delay);
    robot.move_forward(-4, 2);
    robot.defaultArm();
}

void dance(Robot &robot) {
    robot.rotate(2, -90, true);
    for (int i = 0; i < 3; i++) {
        robot.Pause(delay / 5);
        robot.rotate(1, -45, true, 2, 90);
        robot.rotate(1, 45, true, 2, -90);
    }
    robot.rotate(1, 90, true, 2, 90);
    for (int i = 0; i < 10; i++) {
        robot.rotate(0, 45 * (i % 2 * 2 - 1));
        robot.Pause(delay);
        robot.move_forward(1);
        robot.move_forward(-1);
        robot.move_forward(1);
        robot.move_forward(-1);
        robot.turn(60, 1);
        robot.Pause(delay);
    }
}

void runCourse(Robot &robot) { // god function hahahaha bad practice can't stop me
    robot.initialize();
    
    // start button
    robot.move_forward(5, 2); // hit button
    robot.Pause(delay);
    robot.turn(135, -1);
    robot.Pause(delay);

    // initial alignment step so starting position doesn't matter
    robot.move_forward(3, 2);
    robot.Pause(delay);
    robot.turn(90, -1);
    robot.Pause(delay);
    robot.move_forward(-20, 2); // align against right side to guarantee x position
    robot.move_forward(10, 2);
    robot.Pause(delay);
    robot.turn(90, 1);
    robot.Pause(delay);
    robot.move_forward(-20, 2); // align against back to guarantee y position

    // compost
    robot.move_forward(1, 2);
    robot.Pause(delay);
    robot.turn(90, -1);
    robot.Pause(delay);
    robot.move_forward(-2.5, 2);
    robot.stop(); // debug checkpoint
    compost(robot);

    // window
    robot.move_forward(4, 2);
    robot.Pause(delay);
    robot.turn(90, 1);
    robot.Pause(delay);
    robot.move_forward(-5, 2); // align against back
    robot.sprint(12);
    robot.turn(45, -1);
    robot.move_forward(1, 2);
    robot.turn(45, 1);
    robot.move_forward(18, 2); // align against garden front
    robot.move_forward(-3.25, 2);
    robot.stop(); // debug checkpoint
    window(robot);

    // lift bucket
    robot.move_forward(12, 2); // align against garden front
    robot.move_forward(-5.5);
    robot.Pause(delay);
    robot.turn(90, -1);
    robot.Pause(delay);
    robot.move_forward(-0.5, 2);
    robot.stop(); // debug checkpoint
    lift(robot);
    robot.Pause(delay);

    // drop bucket
    robot.turn(90, -1);
    robot.Pause(delay);
    robot.move_forward(5, 2);
    robot.Pause(delay);
    robot.turn(90, -1);
    robot.stop(); // debug checkpoint
    robot.Pause(delay);
    robot.move_forward(26, 2); // align against right side
    robot.turn(90, -1);
    robot.Pause(delay);
    robot.move_forward(26, 2);
    robot.Pause(delay);
    robot.hug(1); // align against right side
    robot.Pause(delay);
    robot.move_forward(20, 2); // align against table front
    robot.stop(); // debug checkpoint
    drop(robot);

    /*
    Trying a new order, keeping this in case it fails
    Needs testing to know which is truly faster
    */

    // // levers
    // robot.turn(90, -1);
    // robot.Pause(delay);
    // robot.move_forward(-9, 2); // align against right side
    // robot.Pause(delay);
    // robot.sprint(14);
    // // robot.move_forward(14, 2);
    // robot.Pause(delay);
    // robot.turn(90, 1);
    // robot.stop(); // debug checkpoint
    // robot.Pause(delay);
    // robot.sprint(12);
    // robot.move_forward(4, 2); // align against bin front
    // robot.Pause(delay);
    // robot.turn(90, -1);
    // robot.Pause(delay);
    // robot.move_forward(2.5, 2);
    // robot.Pause(delay);
    // robot.turn(45, 1);
    // robot.Pause(delay);
    // robot.move_forward(-1, 2);
    // robot.Pause(delay);
    // robot.stop(); // debug checkpoint
    // levers(robot);
    // robot.Pause(delay);
    // robot.stop(); // debug checkpoint

    // // buttons
    // robot.move_forward(16, 1); // might add realignment step between if needed
    // robot.stop(); // debug checkpoint
    // robot.Pause(delay);
    // robot.move_forward(3, 2);
    // robot.Pause(delay);
    // robot.turn(90, 1);
    // robot.Pause(delay);
    // robot.stop(); // debug checkpoint
    // robot.move_forward(-6, 1, false, 20);
    // robot.Pause(delay);
    // robot.stop(); // debug checkpoint
    // buttons(robot);
    // robot.Pause(delay);

    // // stop button
    // robot.sprint(20);
    // robot.move_forward(14, 2); // align against right side
    // robot.Pause(delay);
    // robot.move_forward(-0.6);
    // robot.Pause(delay);
    // robot.turn(90, 1);
    // robot.Pause(delay);
    // robot.move_forward(-6, 2); // align against table front
    // robot.Pause(delay);
    // robot.sprint(22);
    // robot.move_forward(11, 2);
    // robot.Pause(delay);
    // robot.turn(90, 1);
    // robot.Pause(delay);
    // robot.move_forward(-10, 2); // align against right side
    // robot.Pause(delay);
    // robot.sprint(10);
    // // robot.move_forward(10, 2);
    // robot.Pause(delay);
    // robot.turn(135, -1);
    // robot.Pause(delay);
    // robot.move_forward(10, 2); // hit button
    // robot.Pause(delay);
    // robot.stop(); // debug checkpoint

    // buttons
    robot.turn(90, -1);
    robot.move_forward(-9, 2); // align against right side
    robot.sprint(12);
    bool foundLight = robot.move_forward(16, 1);
    robot.stop(); // debug checkpoint
    if (foundLight) { // in case the sensor misses the light and just hits both buttons, idk might remove
        buttons(robot);
    } else {
        robot.move_forward(-3, 2);
        robot.Pause(delay);
        robot.turn(180, 1);
    }
    robot.stop(); // debug checkpoint
    robot.Pause(delay);

    // levers
    robot.move_forward(4, 2);
    robot.Pause(delay);
    robot.turn(90, -1);
    robot.stop(); // debug checkpoint
    robot.Pause(delay);
    robot.sprint(12);
    robot.move_forward(4, 2); // align against bin front
    robot.Pause(delay);
    robot.turn(90, -1);
    robot.Pause(delay);
    robot.move_forward(2.5, 2);
    robot.Pause(delay);
    robot.turn(45, 1);
    robot.Pause(delay);
    robot.move_forward(-0.5, 2);
    robot.stop(); // debug checkpoint
    levers(robot);
    robot.Pause(delay);
    robot.stop(); // debug checkpoint

    // stop button
    robot.sprint(-10);
    robot.turn(45, -1);
    robot.move_forward(-14, 2); // align against right side
    robot.move_forward(0.6, 2);
    robot.turn(90, -1);
    robot.move_forward(-12, 2); // align against table front
    robot.stop(); // debug checkpoint
    robot.sprint(22);
    robot.move_forward(18, 2);
    robot.turn(90, 1);
    robot.move_forward(-10, 2); // align against right side
    robot.sprint(12);
    robot.Pause(delay);
    robot.turn(135, -1);
    robot.Pause(delay);
    robot.move_forward(10, 2); // hit button
    robot.stop(); // debug checkpoint
    
    // victory dance
    robot.move_forward(-6);
    robot.Pause(delay);
    dance(robot);
}