#pragma once

#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHSD.h>
#include <FEH.h>
#include <utils.h>
#include <robot.h>

/**
    README:

    @brief
    Make sure you measure these values across multiple runs on every course (2 - 3 runs per course)
    and adjust accordingly

    You may add more if needed in other locations, just add the appropriate values at the end of each array
    @example Example call for index [3]:
        robot.move_forward(courseSpecificMovements[robot.currentCourse][3], 2);
    Make sure to add such calls to the appropriate locations in "tasks.cpp"

    @details
    -- first value, index [0] --
        determines how far left/right the robot is during the window task
        large value = farther left, small value = farther right

    -- second value, index [1] --
        {Not as significant, change other values first if you can}
        determines how close to the table the robot is AFTER dropping off the bucket
        large value = closer, small value = farther

    -- third value, index [2] --
        determines how close the robot is the to levers
        large value = closer, small value = farther

    -- fourth value, index [3] --
        determines how far left/right the robot is during the buttons task
        large value = farther right, small value = farther left
*/
const float courseSpecificMovements[8][8] = {
    {1.25, 3.7, 2, 4}, // A
    {2.5, 3.7, 2, 4}, // B
    {2.5, 3.7, 2, 4}, // C
    {1.25, 3.7, 2, 4}, // D
    {1.25, 3.7, 2, 4}, // E
    {1.25, 3.7, 2, 4}, // F
    {1.25, 3.7, 2, 4}, // G
    {1.25, 3.7, 2, 4} // H
};

void runCourse(Robot &robot);