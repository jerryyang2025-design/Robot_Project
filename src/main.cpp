#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHSD.h>
#include <FEH.h>

// Declarations for analog optosensors
AnalogInputPin right_opto(FEHIO::Pin2);
AnalogInputPin middle_opto(FEHIO::Pin3);
AnalogInputPin left_opto(FEHIO::Pin4);

void ERCMain()
{
    int x, y;

    //Initialize the screen
    LCD.Clear(BLACK); //clear screen
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("Analog Optosensor Testing");
    LCD.WriteLine("Touch the screen");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    // Record values for optosensors on and off of the straight line
    // Left Optosensor on straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place left optosensor on straight line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (1/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to the screen
    float leftOptosensorValue = left_opto.Value();
    LCD.Write("Left Optosensor Value:");
    LCD.WriteLine(leftOptosensorValue);

    // Left Optosensor off straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place left optosensor off straight line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (2/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to the screen
    
    // <ADD CODE HERE>

    // Repeat process for remaining optosensors, and repeat all three for the curved line values
    // <ADD CODE HERE>

    // Print end message to screen
    LCD.Clear(BLACK);
    LCD.WriteLine("Test Finished");
}