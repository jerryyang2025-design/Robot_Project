#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <controls.h>

void ERCMain()
{
    driveTrain cart;
    int motor_percent = 25; //Input power level here

    int x, y; //for touch screen

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("Shaft Encoder Exploration Test");
    LCD.WriteLine("Touch the screen");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    cart.move_forward(motor_percent, 14);
    cart.turn(motor_percent, 90, -1);
    cart.move_forward(motor_percent, 10);
    cart.turn(motor_percent, 90, 1);
    cart.move_forward(motor_percent, 4);

}