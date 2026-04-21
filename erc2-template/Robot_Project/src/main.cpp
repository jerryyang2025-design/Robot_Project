#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <robot.h>
#include <tasks.h>

// AnalogInputPin test_light_sensor(FEHIO::Pin14);

/*
Main function to instantiate Robot class and complete tasks
*/
void ERCMain()
{
    Robot robot;

    robot.debugTest(3);

    runCourse(robot);

    // robot.stop();
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