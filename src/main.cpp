#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <controls.h>

void ERCMain()
{
    Robot robot;
    int x, y; //for touch screen

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("Shaft Encoder Exploration Test");
    LCD.WriteLine("Touch the screen");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    LCD.WriteLine("3");
    Sleep(1000);
    LCD.WriteLine("2");
    Sleep(1000);
    LCD.WriteLine("1");
    Sleep(1000);

    robot.move_forward(14);
    robot.turn(90, -1);
    robot.move_forward(10);
    robot.turn(90, 1);
    robot.move_forward(4);
    // navigate();
}