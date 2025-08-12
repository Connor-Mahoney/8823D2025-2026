/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       conno                                                     */
/*    Created:      5/29/2025, 8:38:08 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
brain Brain;
controller Controller;
motor LF = motor(vex::PORT11, gearSetting::ratio6_1, true);
motor LM = motor(vex::PORT15, gearSetting::ratio6_1, true);
motor LR = motor(vex::PORT20, gearSetting::ratio6_1, true);
motor RF = motor(vex::PORT1, gearSetting::ratio6_1, false);
motor RM = motor(vex::PORT5, gearSetting::ratio6_1, false);
motor RR = motor(vex::PORT10, gearSetting::ratio6_1, false);
motor LI = motor(vex::PORT3, gearSetting::ratio6_1, false);
motor UI = motor(vex::PORT4, gearSetting::ratio6_1, false);
digital_out tounge = digital_out(Brain.ThreeWirePort.H);
inertial Inertial1 = inertial(vex::PORT1);
inertial Inertial2 = inertial(vex::PORT2);


// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
void toggle_tounge(){
  tounge.set(!tounge.value());
}
//giughiuhi
void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    LF.spin(fwd, Controller.Axis3.value() + Controller.Axis1.value()*.67, pct);
    LM.spin(fwd, Controller.Axis3.value() + Controller.Axis1.value()*.67, pct);
    LR.spin(fwd, Controller.Axis3.value() + Controller.Axis1.value()*.67, pct);
    RF.spin(fwd, Controller.Axis3.value() - Controller.Axis1.value()*.67, pct);
    RM.spin(fwd, Controller.Axis3.value() - Controller.Axis1.value()*.67, pct);
    RR.spin(fwd, Controller.Axis3.value() - Controller.Axis1.value()*.67, pct);

    LF.setStopping(brake);
    LM.setStopping(brake);
    LR.setStopping(brake);
    RF.setStopping(brake);
    RM.setStopping(brake);
    RR.setStopping(brake);

    //intake controlls
    if(Controller.ButtonR1.pressing()){
      LI.spin(fwd, 100, pct);
      UI.spin(fwd, 100, pct);
    }
    else if(Controller.ButtonR2.pressing()){
      LI.spin(reverse, 100, pct);
      UI.spin(reverse, 100, pct);
    }
    else if(Controller.ButtonL1.pressing()){
      LI.spin(fwd, 100, pct);
      UI.spin(reverse, 100, pct);
    }
    else{
      LI.stop(brake);
      UI.stop(brake);
    }

    //tounge controlls
    Controller.ButtonL2.pressed(toggle_tounge);

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
