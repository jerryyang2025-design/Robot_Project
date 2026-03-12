// #include <tasks.h>

// void buttons(Robot robot) { // temp, change once arm is added
//     if (robot.lightColor() == -1) {
//         robot.turn(90, -1);
//         Sleep(200);
//         robot.move_forward(1);
//         Sleep(200);
//         robot.turn(90, 1);
//         Sleep(200);
//         robot.move_forward(3);
//         Sleep(200);
//         robot.move_forward(3, -SPEED);
//         Sleep(200);
//         robot.turn(90, -1);
//         Sleep(200);
//         robot.move_forward(1, -SPEED);
//         Sleep(200);
//         robot.turn(90, 1);
//     } else if (robot.lightColor() == 1) {
//         robot.turn(90, 1);
//         Sleep(200);
//         robot.move_forward(1);
//         Sleep(200);
//         robot.turn(90, -1);
//         Sleep(200);
//         robot.move_forward(3);
//         Sleep(200);
//         robot.move_forward(3, -SPEED);
//         Sleep(200);
//         robot.turn(90, 1);
//         Sleep(200);
//         robot.move_forward(1, -SPEED);
//         Sleep(200);
//         robot.turn(90, -1);
//     }
// }