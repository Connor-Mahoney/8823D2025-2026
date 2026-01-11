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

motor LF = motor(vex::PORT15, gearSetting::ratio6_1, true);
motor LM = motor(vex::PORT13, gearSetting::ratio6_1, false);
motor LR = motor(vex::PORT14, gearSetting::ratio6_1, true);
motor RF = motor(vex::PORT18, gearSetting::ratio6_1, false);
motor RM = motor(vex::PORT17, gearSetting::ratio6_1, true);
motor RR = motor(vex::PORT16, gearSetting::ratio6_1, false);


motor Intake = motor(vex::PORT20, gearSetting::ratio6_1, true);
motor middleRoller = motor(vex::PORT11, gearSetting::ratio18_1, false);
motor topRoller = motor(vex::PORT1, gearSetting::ratio18_1, true);

digital_out tounge = digital_out(Brain.ThreeWirePort.H);
digital_out basket = digital_out(Brain.ThreeWirePort.A);
digital_out wing = digital_out(Brain.ThreeWirePort.E);
inertial Inertial1 = inertial(vex::PORT12);
inertial Inertial2 = inertial(vex::PORT3);

rotation leftRotation = rotation(vex::PORT21);
rotation rightRotation = rotation(vex::PORT19);



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
bool fin;
bool evan = 0;

void pre_auton(void)
{

  Inertial1.calibrate();
//   while (Inertial1.isCalibrating())
//   {
//     Brain.Screen.clearScreen();
//     Brain.Screen.print("inertial 1 calibrating\n");
//     wait(50, msec);
//   }
//   Inertial2.calibrate();
//   while (Inertial2.isCalibrating())
//   {
//     Brain.Screen.clearScreen();
//     Brain.Screen.print("inertial 2 calibrating\n");
//     wait(50, msec);
//   }
//   Controller.Screen.clearScreen();
//   Controller.Screen.setCursor(1, 1);
//   Controller.Screen.print("a = left\n");
//   Controller.Screen.setCursor(2, 1);
//   Controller.Screen.print("b = right\n");
//   Controller.Screen.setCursor(3, 1);
//   Controller.Screen.print("x = skills");
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

void intakeBasket(float speed);
void outtake(float speed);
void scoreHigh(float speed);
void scoreMiddle(float speed);
void stopIntake();


void autonomous(void)
{
  
  if (baleft)
  {
    thread([]()
    {
      // outtake(25);
      // wait(1, sec);
      intakeBasket(100);
      wait(2.25, sec);
      stopIntake();       
    }).detach();
    drive(28, 50);//pick up
    wait(50, msec);
    turn(-105);
    wait(50, msec);
    drive(-14, 30);//drive to goal
    wait(50, msec);
    scoreMiddle(100);
    wait(5, sec);
    // turn(2);
    drive(52, 50);//drive away from goal
    stopIntake();
    wait(50, msec);
    turn(-45);
    tounge.set(true);
    
    printf("tounge value = %s\n", tounge.value()? "true":"false");
    wait(0.5, sec);
    thread([]()
    {
      intakeBasket(100);
      wait(2, sec);
      scoreHigh(100);
      printf("tounge value = %s\n", tounge.value()? "true":"false");
    })
    .detach();
     drive(13, 25);
    wait(1, sec);
     drive(-3, 50);
    //  turn(3);
     drive(-24, 50);
    // // scoreHigh(100);

  }
  if (balright)
  {
    thread([]()
    {
      // outtake(25);
      // wait(1, sec);
      intakeBasket(100);
      wait(2.25, sec);
      stopIntake();       
    }).detach();
    drive(28, 50);//pick up
    wait(50, msec);
    turn(-69);
    wait(50, msec);
    drive(15, 30);//drive to goal
    wait(50, msec);
    outtake(100);
    wait(2, sec);
    // turn(2);
    drive(-55, 50);//drive away from goal
    stopIntake();
    wait(50, msec);
    turn(-134);
    tounge.set(true);
    
    printf("tounge value = %s\n", tounge.value()? "true":"false");
    wait(0.5, sec);
    thread([]()
    {
      intakeBasket(100);
      wait(2, sec);
      scoreHigh(100);
      printf("tounge value = %s\n", tounge.value()? "true":"false");
    })
    .detach();
     drive(13, 25);
    wait(1, sec);
     drive(-3, 50);
    //  turn(3);
     drive(-24, 50);
    // // scoreHigh(100);
  }
  if(fin){
    // drive(29, 75);
    // turn(-95);
    // tounge.set(true);
    // intakeBasket(100);
    // wait(0.5, sec);
    // drive(10, 30);
    // wait(5, sec);
    drive(-10, 100);
    
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
bool sdiybt = 1;
void a()
{
  baleft = 1;
  balright = 0;
  Controller.Screen.clearScreen();
  Controller.Screen.setCursor(1, 1);
  Controller.Screen.print("left auto selected");
  evan = 1;
}

void b()
{
  baleft = 0;
  balright = 1;
  Controller.Screen.clearScreen();
  Controller.Screen.setCursor(1, 1);
  Controller.Screen.print("right auto selected");
  evan = 1;
}
void x()
{
  baleft = 0;
  balright = 0;
  fin = 1;
  // Controller.Screen.clearScreen();
  // Controller.Screen.setCursor(1, 1);
  // Controller.Screen.print("skills auto selected");
  evan = 1;
}
void toggle_tounge()
{
  tounge.set(!tounge.value());
}
void toggle_wing(){
  wing.set(!wing.value());
}
void intakeBasket(float speed){
  basket.set(true);
  wing.set(true);
  Intake.spin(fwd, speed, pct);
  middleRoller.spin(fwd, speed, pct);
  topRoller.spin(fwd, speed, pct);
  
}
void outtake(float speed){
  Intake.spin(reverse, speed, pct);
  middleRoller.spin(reverse, speed, pct);
  topRoller.spin(reverse, speed, pct);
}
void scoreHigh(float speed){
  basket.set(false);
  wing.set(false);
  Intake.spin(fwd, speed, pct);
  middleRoller.spin(fwd, speed, pct);
  topRoller.spin(reverse, speed, pct);
  

}
void scoreMiddle(float speed){
  wing.set(false);
  basket.set(false);
  Intake.spin(fwd, speed, pct);
  middleRoller.spin(reverse, speed, pct);
  topRoller.spin(fwd, speed, pct);
  
}
void stopIntake(){
  Intake.stop();
  middleRoller.stop();
  topRoller.stop();
}
void toggle_intake()
{
  sdiybt = 1 - sdiybt;
  printf("sdiybt = %d\n", sdiybt);
  if(!sdiybt){
      // Controller.Screen.setCursor(3, 1);
      // Controller.Screen.clearScreen();
      // Controller.Screen.print("score middle goal");
    }

      if(sdiybt){
      // Controller.Screen.clearScreen();
      // Controller.Screen.setCursor(3, 1);
      // Controller.Screen.print("score top goal");
    }
}

// giughiuhi
void usercontrol(void)
{
  // User control code here, inside the loop
  while (1)
  {
      if(!evan){
        Controller.ButtonA.pressed(a);
      Controller.ButtonB.pressed(b);
      Controller.ButtonX.pressed(x);
      }
      Controller.ButtonRight.pressed(toggle_wing);
      
    
Controller.ButtonY.pressed(toggle_intake);
    
    
    
     
  

    
     
    
    
    LF.spin(fwd, Controller.Axis3.value() + Controller.Axis1.value() * .41, pct);
    LM.spin(fwd, Controller.Axis3.value() + Controller.Axis1.value() * .41, pct);
    LR.spin(fwd, Controller.Axis3.value() + Controller.Axis1.value() * .41, pct);
    RF.spin(fwd, Controller.Axis3.value() - Controller.Axis1.value() * .41, pct);
    RM.spin(fwd, Controller.Axis3.value() - Controller.Axis1.value() * .41, pct);
    RR.spin(fwd, Controller.Axis3.value() - Controller.Axis1.value() * .41, pct);

    LF.setStopping(brake);
    LM.setStopping(brake);
    LR.setStopping(brake);
    RF.setStopping(brake);
    RM.setStopping(brake);
    RR.setStopping(brake);

    // intake controlls
    
    

    if (Controller.ButtonR1.pressing())
    {
      // LI.spin(fwd, 100, pct);
      // UI.spin(fwd, 100, pct);
      intakeBasket(100);
    }
    else if (Controller.ButtonR2.pressing())
    {
      outtake(100);
    }
    else if (Controller.ButtonL1.pressing())
    {
      if(sdiybt){
        scoreHigh(100);
      }
      if(!sdiybt){
        scoreMiddle(100);
      }
    }
    else
    {
      stopIntake();
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
