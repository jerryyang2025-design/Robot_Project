#include <tasks.h>

/*
Runs the compost task sequence
*/
void compost(Robot &robot) {
    robot.rotate(2, -90);
    robot.rotate(0, -90, true, 1, -20);
    // robot.rotate((int8_t[]){0, 1, 2}, (int16_t[]){-90, -20, -90}, 3, true);
    robot.rotate(2, 20); // finishes positioning
    robot.move_forward(0.5, 2); // moves the arm under the compost
    robot.rotate(1, 20, true, 2, -20); // rotate 90
    robot.stop(); // debug checkpoint
    robot.rotate(0, 120, true, 1, 32); // rotate 180
    robot.Pause(500);
    robot.rotate(0, -90, true, 1, 20); // rotate back 180
    robot.rotate(1, -30, true, 2, 30); // rotate back 45
    robot.move_forward(-2, 2); // clearance
    robot.rotate(0, 70, true);
    robot.rotate(1, -25, false, 2, 25);
    robot.move_forward(2, 2); // positioning
    robot.rotate(0, -15, true, 2, 20); // finish rotating
    robot.Pause(delay);
    robot.move_forward(-2, 2); // clearance
    robot.defaultArm();
    robot.move_forward(2, 2); // move back
}

/*
Runs the bucket lifting sequence
*/
void lift(Robot &robot) {
    robot.rotate(1, 20, false, 2, -28); // positioning
    robot.move_forward(5);
    robot.rotate(2, -10, true);
    robot.rotate(2, -20, true, 1, 90); // lift
    robot.rotate(0, 2, true); // for balance
}

/*
Runs the bucket dropping sequence
*/
void drop(Robot &robot) {
    robot.rotate(0, 23, true, 2, -90); // drop
    robot.Pause(500);
    robot.move_forward(-4, 2);
    robot.defaultArm();
    robot.move_forward(courseSpecificMovements[robot.currentCourse][1], 2);
}

/*
Runs the lever sequence
*/
void levers(Robot &robot) {
    int8_t lever = robot.lever();
    const float distBetweenLevers = 4.5;
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
    robot.rotate(1, -60);
    robot.move_forward(2, 2); // move arm under lever
    robot.Pause(4250); // competition requirement
    robot.rotate(1, -5, true); // push lever back up
    robot.move_forward(-5, 2);
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
        robot.move_forward(distBetweenLevers, 2);
        robot.turn(90, -lever);
    }
}

/*
Runs the buttons sequence
*/
void buttons(Robot &robot) {
    robot.move_forward(0.5, 0, false, 20);
    robot.Pause(delay);
    robot.stop("Check Position");

    int8_t mode = 0;
    int8_t i = 0;
    while (true) {
        for (int i = 0; i < 15; i++) { // read 15 light values
            mode += robot.lightColor();
            robot.move_forward((i % 2 * 2 - 1) * i / 50.0f, 2, false, 20); // moves a bit so it's not all the same value
            robot.Pause(delay / 5);
        }

        robot.Pause(delay / 5);
        LCD.Clear(BLACK);
        LCD.SetFontColor(WHITE);
        LCD.WriteLine("Light Color:");
        if (mode < -5) { // check threshold for confidence
            LCD.Clear(BLUE);
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("Light Color:");
            LCD.WriteLine("Blue");
            mode = -1;
            break;
        } else if (mode > 5) {
            LCD.Clear(RED);
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
            LCD.Clear(RED);
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
    robot.move_forward(6.5, 2);
    robot.Pause(delay);
    robot.move_forward(-3);
    robot.Pause(delay);
    robot.turn(90, mode);
    robot.Pause(delay);
    robot.move_forward(-3);
    robot.Pause(delay);
    robot.turn(90, -mode);
}

/*
Runs the window sequence
*/
void window(Robot &robot) {
    robot.rotate(0, -50);
    robot.rotate(1, 70, true, 2, -45); // positioning
    robot.rotate(0, 0, true, 2, -90);
    robot.rotate(0, 47, true, 2, -70); // open window
    robot.Pause(delay);
    robot.rotate(0, 20, true);
    robot.rotate(1, 90, true, 2, 0); // clearance
    robot.rotate(0, 55, true);
    robot.rotate(1, 70, true, 2, -55); // positioning
    robot.rotate(0, 0, true, 2, -90);
    robot.rotate(0, -34, true, 2, -70); // close window
    robot.Pause(delay);
    robot.rotate(0, 0, true);
    robot.move_forward(-4, 2);
    robot.defaultArm();
}

/*
Runs a dance sequence for fun
*/
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

/*
Main sequence to run through the entire course
*/
void runCourse(Robot &robot) { // god function hahahaha bad practice can't stop me
    robot.initialize();
    
    // start button
    robot.move_forward(5, 2); // hit button
    robot.Pause(delay);
    robot.turn(135, -1);
    robot.Pause(delay);

    // initial alignment step so starting position doesn't matter
    robot.move_forward(3, 2);
    robot.turn(90, -1);
    robot.move_forward(-12, 2); // align against right side to guarantee x position
    robot.sprint(10);
    robot.Pause(delay);
    robot.turn(90, 1);
    robot.defaultArm();
    robot.move_forward(-17, 2); // align against back to guarantee y position

    // compost
    robot.move_forward(1.2, 2);
    robot.Pause(delay);
    robot.turn(96, -1);
    robot.Pause(delay);
    robot.move_forward(-2.7, 2);
    robot.stop("Compost"); // debug checkpoint
    compost(robot);

    // window
    robot.move_forward(6, 2);
    robot.move_forward(-1, 2);
    robot.Pause(delay);
    robot.turn(90, 1);
    robot.Pause(delay);
    robot.move_forward(-5, 2); // align against back
    robot.sprint(12);
    robot.Pause(delay);
    robot.turn(45, -1);
    robot.Pause(delay);
    robot.move_forward(courseSpecificMovements[robot.currentCourse][0], 2);
    robot.Pause(delay);
    robot.turn(45, 1);
    robot.Pause(delay);
    robot.move_forward(15, 2); // align against garden front
    robot.move_forward(-4, 2);
    robot.stop("Window"); // debug checkpoint
    window(robot);

    // lift bucket
    robot.move_forward(12, 2); // align against garden front
    robot.move_forward(-4.75);
    robot.Pause(delay);
    robot.turn(90, -1);
    robot.Pause(delay);
    robot.move_forward(-2, 2);
    robot.stop("Lift Bucket"); // debug checkpoint
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
    robot.sprint(12);
    robot.move_forward(6, 2); // align against right side
    robot.move_forward(-1, 2);
    robot.turn(92, -1);
    robot.Pause(delay);
    robot.move_forward(32, 2);
    robot.Pause(delay);
    robot.hug(1); // align against right side
    robot.Pause(delay);
    robot.turn(3, 1);
    robot.rotate(0, 0, true);
    robot.move_forward(6, 2);
    robot.move_forward(-1.5, 2);
    robot.stop("Drop Bucket"); // debug checkpoint
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

    // buttons
    robot.turn(90, -1);
    robot.move_forward(-9, 2); // align against right side
    robot.sprint(3);
    robot.Pause(delay);
    robot.turn(30, 1);
    robot.Pause(delay);
    robot.sprint(courseSpecificMovements[robot.currentCourse][3]);
    robot.Pause(delay);
    robot.turn(30, -1);
    robot.Pause(delay);
    robot.sprint(5);
    bool foundLight = robot.move_forward(16, 1);
    robot.stop("Buttons"); // debug checkpoint
    if (foundLight) { // in case the sensor misses the light and just hits both buttons, idk might remove
        buttons(robot);
    } else {
        LCD.Clear(RED); // idk just act like it detected red, 50/50 chance to get points
        robot.move_forward(-3, 2);
    }
    robot.stop("Buttons Finished"); // debug checkpoint
    robot.Pause(delay);

    // levers
    robot.sprint(-20);
    robot.move_forward(-4, 2);
    robot.sprint(12);
    robot.Pause(delay);
    robot.turn(90, 1);
    robot.stop(); // debug checkpoint
    robot.Pause(delay);
    robot.sprint(12);
    robot.move_forward(4, 2); // align against bin front
    robot.Pause(delay);
    robot.turn(90, -1);
    robot.Pause(delay);
    robot.move_forward(courseSpecificMovements[robot.currentCourse][2], 2);
    robot.Pause(delay);
    robot.turn(45, 1);
    robot.Pause(delay);
    robot.move_forward(-1, 2);
    robot.stop("Levers"); // debug checkpoint
    levers(robot);
    robot.Pause(delay);
    robot.stop("Levers Finished"); // debug checkpoint

    // stop button
    robot.sprint(-10);
    robot.turn(45, -1);
    robot.move_forward(-14, 2); // align against right side
    robot.move_forward(2, 2);
    robot.turn(90, -1);
    robot.move_forward(-12, 2); // align against table front
    robot.stop(); // debug checkpoint
    robot.sprint(32);
    robot.move_forward(8, 2);
    robot.turn(90, 1);
    robot.move_forward(-10, 2); // align against right side
    robot.sprint(12);
    robot.Pause(delay);
    robot.turn(135, -1);
    robot.Pause(delay);
    robot.sprint(10); // hit button to end run
    robot.stop("Course Done"); // debug checkpoint
    
    // victory dance
    robot.move_forward(-6);
    robot.Pause(delay);
    dance(robot);
}