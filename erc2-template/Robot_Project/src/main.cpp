#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <robot.h>
#include <tasks.h>

AnalogInputPin test_light_sensor(FEHIO::Pin2);

void ERCMain()
{
    Robot robot;
    robot.stop();

    // int x, y;
    // LCD.Clear(BLACK);
    // LCD.SetFontColor(WHITE);
    // LCD.WriteLine("Base:");
    // while(!LCD.Touch(&x,&y));
    // servosTest[0].TouchCalibrate();
    // while(LCD.Touch(&x,&y));

    // LCD.Clear(BLACK);
    // LCD.SetFontColor(WHITE);
    // LCD.WriteLine("Joint One:");
    // while(!LCD.Touch(&x,&y));
    // servosTest[1].TouchCalibrate();
    // while(LCD.Touch(&x,&y));

    // LCD.Clear(BLACK);
    // LCD.SetFontColor(WHITE);
    // LCD.WriteLine("Joint Two:");
    // while(!LCD.Touch(&x,&y));
    // servosTest[2].TouchCalibrate();
    // while(LCD.Touch(&x,&y));


    robot.defaultArm();
    while (!robot.detect(1));
    
    robot.move_forward(5, 2);
    Sleep(delay);
    robot.turn(135, -1);
    Sleep(delay);

    robot.move_forward(9, 2);
    Sleep(delay);
    robot.hug(1);
    Sleep(delay);
    robot.turn(90, -1);
    Sleep(delay);
    robot.move_forward(12, 2);
    Sleep(delay);
    robot.turn(90, 1);
    Sleep(delay);
    robot.move_forward(24, 2);
    Sleep(delay);
    robot.move_forward(-4, 2);
    Sleep(delay);
    robot.turn(90, -1);
    // robot.stop();

    robot.rotate(1, 20, false, 2, -32);
    // robot.stop();

    Sleep(delay);
    robot.move_forward(4);
    Sleep(delay);
    robot.rotate(2, -10, true);
    Sleep(delay);
    robot.rotate(2, -40, true, 1, 90);
    Sleep(delay);
    // robot.stop();

    // drop bucket
    robot.turn(90, -1);
    Sleep(delay);
    robot.move_forward(5, 2);
    Sleep(delay);
    robot.turn(90, -1);
    // robot.stop(); // checkpoint
    Sleep(delay);
    robot.move_forward(36, 2);
    Sleep(delay);
    robot.move_forward(1, 2);
    Sleep(delay);
    robot.turn(82, -1);
    Sleep(delay);
    robot.move_forward(24, 2);
    Sleep(delay);
    robot.hug(1);
    Sleep(delay);
    robot.move_forward(24, 2);
    Sleep(delay);
    robot.move_forward(2, 2);
    Sleep(delay);
    robot.rotate(2, -80);
    Sleep(delay);
    robot.move_forward(-2.5, 2);
    robot.defaultArm();
    // robot.stop();

    Sleep(delay);
    robot.turn(90, -1);
    Sleep(delay);
    robot.move_forward(-9, 2);
    Sleep(delay);
    robot.move_forward(14, 2);
    Sleep(delay);
    robot.turn(90, 1);
    // robot.stop(); // checkpoint
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
    // robot.stop();

    Sleep(delay);
    robot.rotate(1, -20, true);
    robot.move_forward(-2, 2);
    // robot.stop();
    Sleep(delay);
    robot.rotate(1, -60);
    // robot.stop();
    robot.move_forward(2, 2);
    // robot.stop();
    Sleep(delay);
    robot.rotate(1, -5, true);
    Sleep(delay);
    robot.move_forward(-5, 2);
    robot.defaultArm();
    dance(robot);


    // runCourse(robot);


    // int x, y;
    // float min = 5, max = 0;
    // float test;
    // LCD.Clear(BLACK);
    // LCD.SetFontColor(WHITE);
    // LCD.WriteLine("Blue:");
    // while(!LCD.Touch(&x,&y)) {
    //     test = test_light_sensor.Value();
    //     LCD.WriteLine(test);
    //     if (test < min) {
    //         min = test;
    //     } else if (test > max) {
    //         max = test;
    //     }
    //     Sleep(delay);
    // }
    // while(LCD.Touch(&x,&y));
    // LCD.Clear(BLACK);
    // LCD.SetFontColor(WHITE);
    // LCD.WriteLine("Blue Min:");
    // LCD.WriteLine(min);
    // LCD.WriteLine("Blue Max:");
    // LCD.WriteLine(max);

    // while(!LCD.Touch(&x,&y));
    // while(LCD.Touch(&x,&y));

    // min = 5, max = 0;
    // LCD.Clear(BLACK);
    // LCD.SetFontColor(WHITE);
    // LCD.WriteLine("Red:");
    // while(!LCD.Touch(&x,&y)) {
    //     test = test_light_sensor.Value();
    //     LCD.WriteLine(test);
    //     if (test < min) {
    //         min = test;
    //     } else if (test > max) {
    //         max = test;
    //     }
    //     Sleep(delay);
    // }
    // while(LCD.Touch(&x,&y));
    // LCD.Clear(BLACK);
    // LCD.SetFontColor(WHITE);
    // LCD.WriteLine("Red Min:");
    // LCD.WriteLine(min);
    // LCD.WriteLine("Red Max:");
    // LCD.WriteLine(max);
}