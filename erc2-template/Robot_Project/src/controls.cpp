/*
    {RETIRED FILE}
*/

// #include <controls.h>

// // motors
// FEHMotor left_motor(FEHMotor::Motor1, 9);
// FEHMotor right_motor(FEHMotor::Motor0, 9);

// // sensors
// DigitalEncoder right_encoder(FEHIO::Pin8);
// DigitalEncoder left_encoder(FEHIO::Pin9);
// DigitalInputPin left_switch(FEHIO::Pin10); // back
// DigitalInputPin right_switch(FEHIO::Pin11);
// AnalogInputPin left_opto(FEHIO::Pin0);
// AnalogInputPin right_opto(FEHIO::Pin1);
// AnalogInputPin light_sensor(FEHIO::Pin3);

// // servos
// FEHServo base(FEHServo::Servo0);
// FEHServo joint1(FEHServo::Servo1);
// FEHServo joint2(FEHServo::Servo2);
// FEHServo servos[3] = {base, joint1, joint2};

// void Robot::move_forward(int8_t inches, int8_t percent, int8_t early) { // change to accept negative distance
//     //Reset encoder counts
//     right_encoder.ResetCounts();
//     left_encoder.ResetCounts();

//     float counts = inches * 40.5f;

//     //Set both motors to desired percent
//     right_motor.SetPercent(percent);
//     left_motor.SetPercent(percent);

//     //While the average of the left and right encoder is less than counts,
//     //keep running motors
//     while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

//     //Turn off motors
//     right_motor.Stop();
//     left_motor.Stop();
// }

// void Robot::turn(int16_t degrees, int8_t direction, int8_t percent) { // positive: turn right, negative: turn left
//     //Reset encoder counts
//     right_encoder.ResetCounts();
//     left_encoder.ResetCounts();

//     const float radius = 3.35; // inches
//     const float pi = 3.141592653589793238462643383;

//     float counts = radius * 40.5f * degrees * pi / 180.0f;

//     //Set both motors to desired percent
//     right_motor.SetPercent(-direction * percent);
//     left_motor.SetPercent(direction * percent);

//     //While the average of the left and right encoder is less than counts,
//     //keep running motors
//     while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

//     //Turn off motors
//     right_motor.Stop();
//     left_motor.Stop();
// }

// void Robot::follow(FEHMotor motor1, FEHMotor motor2) {
//     motor2.SetPercent(SPEED * 0.2);
//     motor1.SetPercent(SPEED * 1.4); // why did I have 4 lines here again? should probably test and fix it...
// }

// bool Robot::detect(int8_t type) { // 0 = no detection, 1 = microswitches, 2 = light, 3 = line
//     if (type == 0) {
//         return false;   
//     }
//     bool detected = false;
//     if (type == 1) {
//         bool left_detected = !left_switch.Value();
//         bool right_detected = !right_switch.Value();
//         if (left_detected && right_detected) {
//             detected = true;
//         }
//     } else if ( type == 2) {
//         if (lightColor() != 0) {
//             detected = true;
//         }
//     } else if (type == 3) {
//         bool left_detected = left_opto.Value() > left_opto_threshold;
//         bool right_detected = right_opto.Value() > right_opto_threshold;
//         if (left_detected || right_detected) {
//             detected = true;
//         }
//     }
//     return detected;
// }

// void Robot::controlledFollow(int8_t inches, int8_t direction, int8_t early) {
//     right_encoder.ResetCounts();
//     left_encoder.ResetCounts();
//     float counts = inches * 40.5f;
//     while (!detect(early) && (left_encoder.Counts() + right_encoder.Counts()) / 2. < counts) {
//         bool left_detected = left_opto.Value() > left_opto_threshold;
//         bool right_detected = right_opto.Value() > right_opto_threshold;
//         bool both = right_detected && left_detected;
//         if (direction > 0 && both) {
//             turn(10, direction);
//             continue;
//         }
//         if (direction < 0 && both) {
//             turn(10, direction);
//             continue;
//         }

//         if (left_detected) {
//             follow(right_motor, left_motor);
//         } else if (right_detected) {
//             follow(left_motor, right_motor);
//         } else {
//             left_motor.SetPercent(SPEED);
//             right_motor.SetPercent(SPEED);
//         }
//     }
//     LCD.WriteLine("Loop Exited");
//     right_motor.Stop();
//     left_motor.Stop();
// }

// void Robot::rotate(int8_t joint, int16_t angle) {
//     int increment = (angle - angles[joint]) / rotateIncrement;
//     for (int i = 0; i < rotateIncrement; i++) {
//         servos[joint].SetDegree(angles[joint] + increment * (i + 1));
//     }
//     angles[joint] = angle;
// }

// void Robot::defaultArm() {
//     for (int i = 0; i < 3; i++) {
//         rotate(i, 0);
//     }
// }

// void Robot::stop() {
//     int x, y;
//     LCD.Clear(BLACK);
//     LCD.SetFontColor(WHITE);
//     LCD.WriteLine("Paused");
//     LCD.WriteLine("Touch the screen to continue");
//     while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
//     while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
//     LCD.WriteLine("3");
//     Sleep(1000);
//     LCD.WriteLine("2");
//     Sleep(1000);
//     LCD.WriteLine("1");
//     Sleep(1000);
// }

// int8_t Robot::lightColor() { // 0 = no light, -1 = blue, 1 = red
//     if (light_sensor.Value() < 0.5) {
//         return 1;
//     } else if (light_sensor.Value() < 2) {
//         return -1;
//     } else {
//         return 0;
//     }
// }

// void Robot::tempMove(float time, int8_t dir) {
//     if (dir > 0) {
//         right_motor.SetPercent(-SPEED);
//         left_motor.SetPercent(SPEED);
//     } else {
//         right_motor.SetPercent(SPEED);
//         left_motor.SetPercent(-SPEED);
//     }
//     Sleep(time);
//     right_motor.Stop();
//     left_motor.Stop();
// }

// void Robot::check_x(int8_t speed, float x_coordinate, int8_t orientation) {
//     int power = speed;
//     if(orientation == -1){
//         power = -speed;
//     }

//     RCSPose* pose = RCS.RequestPosition();

//     while (pose->x < x_coordinate - 1 || pose->x > x_coordinate + 1) {
//         if(pose->x < x_coordinate - 1) {
//             move_forward(1, power);
//         } else if (pose->x > x_coordinate + 1) {
//             move_forward(-1, power);
//         }
//         Sleep(100);

//         pose = RCS.RequestPosition();
//     }
// }


// /* 
//  * Use RCS to move to the desired y_coordinate based on the orientation of the QR code
//  */
// void Robot::check_y(int8_t speed, float y_coordinate, int8_t orientation) {
//     int power = speed;
//     if(orientation == -1){
//         power = -speed;
//     }

//     RCSPose* pose = RCS.RequestPosition();

//     while (pose->y < y_coordinate - 1 || pose->y > y_coordinate + 1) {
//         if(pose->y < y_coordinate - 1) {
//             move_forward(1, power);
//         } else if (pose->y > y_coordinate + 1) {
//             move_forward(-1, power);
//         }
//         Sleep(100);

//         pose = RCS.RequestPosition();
//     }
// }

// void Robot::check_heading(float heading) {
//     RCSPose* pose = RCS.RequestPosition();

//     while (pose->heading < heading - 1 || pose->heading > heading + 1) {
//         if(pose->heading < heading - 1) {
//             turn(1, -1);
//         } else if (pose->heading > heading + 1) {
//             turn(1, 1);
//         }
//         Sleep(100);

//         pose = RCS.RequestPosition();
//     }
// }