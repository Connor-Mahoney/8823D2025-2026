/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       conno                                                     */
/*    Created:      5/29/2025, 8:38:08 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "useful.h"

using namespace vex;

// A global instance of competition
competition Competition;
brain Brain;
controller Controller;
motor LF = motor(vex::PORT11, gearSetting::ratio6_1, true);
motor LM = motor(vex::PORT15, gearSetting::ratio6_1, true);
motor LR = motor(vex::PORT19, gearSetting::ratio6_1, true);
motor RF = motor(vex::PORT1, gearSetting::ratio6_1, false);
motor RM = motor(vex::PORT18, gearSetting::ratio6_1, false);
motor RR = motor(vex::PORT17, gearSetting::ratio6_1, false);
motor LI = motor(vex::PORT3, gearSetting::ratio6_1, false);
motor UI = motor(vex::PORT4, gearSetting::ratio6_1, false);
digital_out tounge = digital_out(Brain.ThreeWirePort.H);
digital_out toungeR = digital_out(Brain.ThreeWirePort.A);
inertial Inertial1 = inertial(vex::PORT16);
inertial Inertial2 = inertial(vex::PORT9);

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

bool baleft;
bool balright;

void pre_auton(void)
{

  Inertial1.calibrate();
  while (Inertial1.isCalibrating())
  {
    Brain.Screen.clearScreen();
    Brain.Screen.print("inertial 1 calibrating\n");
    wait(50, msec);
  }
  Inertial2.calibrate();
  while (Inertial2.isCalibrating())
  {
    Brain.Screen.clearScreen();
    Brain.Screen.print("inertial 2 calibrating\n");
    wait(50, msec);
  }
  Controller.Screen.clearScreen();
  Controller.Screen.setCursor(1, 1);
  Controller.Screen.print("a = left\n");
  Controller.Screen.setCursor(2, 1);
  Controller.Screen.print("b = right\n");
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

void autonomous(void)
{
  if (baleft)
  {
  }
  if (balright)
  {
    thread([]()
           {
             LI.spin(fwd, 30, pct);
             UI.spin(fwd, 30, pct);
             wait(2, sec);
             LI.stop();
             UI.stop();
           })
        .detach();
    drive(27, 20);
    wait(50, msec);
    turn(-75);
    wait(50, msec);
    drive(16, 20);
    wait(50, msec);
    LI.spin(reverse, 75, pct);
    UI.spin(reverse, 75, pct);
    wait(1.5, sec);
    LI.stop();
    UI.stop();
    wait(0.5, sec);
    drive(-49, 50);
    wait(50, msec);
    turn(-138);
    tounge.set(true);
    toungeR.set(true);
    wait(0.5, sec);
    thread([]()
    {
      LI.spin(fwd, 30, pct);
      UI.spin(fwd, 30, pct);
      wait(3, sec);
      LI.stop();
      UI.stop();
    })
    .detach();
    drive(8.5, 50);
    wait(1, sec);
    drive(-3, 50);
    // turn(5);
    drive(-25, 50);
    LI.spin(fwd, 100, pct);
    UI.spin(fwd, 100, pct);
  }
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
bool sdiybt = 0;
void a()
{
  baleft = 1;
  balright = 0;
  Controller.Screen.clearScreen();
  Controller.Screen.setCursor(1, 1);
  Controller.Screen.print("left auto selected");
}
void b()
{
  baleft = 0;
  balright = 1;
  Controller.Screen.clearScreen();
  Controller.Screen.setCursor(1, 1);
  Controller.Screen.print("right auto selected");
}
void toggle_tounge()
{
  tounge.set(!tounge.value());
  toungeR.set(!tounge.value());
}
void toggle_intake()
{
  sdiybt = 1;
}
void toggle_intake2()
{
  sdiybt = 0;
}
// giughiuhi
void usercontrol(void)
{
  // User control code here, inside the loop
  while (1)
  {
    Controller.ButtonA.pressed(a);
    Controller.ButtonB.pressed(b);

    LF.spin(fwd, Controller.Axis3.value() + Controller.Axis1.value() * .5067, pct);
    LM.spin(fwd, Controller.Axis3.value() + Controller.Axis1.value() * .5067, pct);
    LR.spin(fwd, Controller.Axis3.value() + Controller.Axis1.value() * .5067, pct);
    RF.spin(fwd, Controller.Axis3.value() - Controller.Axis1.value() * .5067, pct);
    RM.spin(fwd, Controller.Axis3.value() - Controller.Axis1.value() * .5067, pct);
    RR.spin(fwd, Controller.Axis3.value() - Controller.Axis1.value() * .5067, pct);

    LF.setStopping(brake);
    LM.setStopping(brake);
    LR.setStopping(brake);
    RF.setStopping(brake);
    RM.setStopping(brake);
    RR.setStopping(brake);

    // intake controlls
    Controller.ButtonR1.pressed(toggle_intake);
    Controller.ButtonR2.pressed(toggle_intake2);
    if (sdiybt)
    {
      LI.spin(fwd, 25, pct);
      UI.spin(fwd, 15, pct);
    }
    if (Controller.ButtonR1.pressing())
    {
      LI.spin(fwd, 100, pct);
      UI.spin(fwd, 100, pct);
    }
    else if (Controller.ButtonR2.pressing())
    {
      LI.spin(reverse, 100, pct);
      UI.spin(reverse, 100, pct);
    }
    else if (Controller.ButtonL1.pressing())
    {
      LI.spin(fwd, 67, pct);
      UI.spin(reverse, 67, pct);
    }
    else
    {
      UI.spin(fwd, 10, pct);
      if (!sdiybt)
      {
        UI.stop(brake);
        LI.stop(brake);
      }
    }

    // tounge controlls
    Controller.ButtonL2.pressed(toggle_tounge);

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main()
{
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true)
  {
    wait(100, msec);
  }
}
