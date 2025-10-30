#include "vex.h"
#include "useful.h"

using namespace vex;
extern motor LF;
extern motor LM;
extern motor LR;
extern motor RF;
extern motor RM;
extern motor RR;
extern motor LI;
extern motor UI;
extern inertial Inertial1;
extern inertial Inertial2;
//extern vex::rotation Rotation;
void spin_motorsT(float speed)
{
    LF.spin(forward, speed, pct);
    LM.spin(forward, speed, pct);
    LR.spin(forward, speed, pct);
    RF.spin(reverse, speed, pct);
    RM.spin(reverse, speed, pct);
    RR.spin(reverse, speed, pct);
}
void spin_motorsF(float speed)
{
    LF.spin(forward, speed, pct);
    LM.spin(forward, speed, pct);
    LR.spin(forward, speed, pct);
    RF.spin(forward, speed, pct);
    RM.spin(forward, speed, pct);
    RR.spin(forward, speed, pct);
}
void leftSide(float speed){
    LF.spin(fwd, speed, pct);
    LM.spin(fwd, speed, pct);
    LR.spin(fwd, speed, pct);
}
void rightSide(float speed){
    RF.spin(fwd, speed, pct);
    RM.spin(fwd, speed, pct);
    RR.spin(fwd, speed, pct);
}
void stop_motors()
{
    LF.stop(brake);
    LM.stop(brake);
    LR.stop(brake);
    RF.stop(brake);
    RM.stop(brake);
    RF.stop(brake);
}
void drive(float inches, float speed)
{
    float degPerRotation = (360 / (M_PI * 3.25)) * (4.0 / 3.0);
    printf("deg per rotation: %f\n", degPerRotation);
    float targetRotation = inches * degPerRotation;
    printf("target rotation: %f\n", targetRotation);
    LF.resetPosition();
    RF.resetPosition();
    /*
    LF.spinFor(forward, a * 360, deg, speed, velocityUnits::pct);
    LM.spinFor(forward, a * 360, deg, speed, velocityUnits::pct);
    LR.spinFor(forward, a * 360, deg, speed, velocityUnits::pct);
    RF.spinFor(forward, a * 360, deg, speed, velocityUnits::pct);
    RM.spinFor(forward, a * 360, deg, speed, velocityUnits::pct);
    RF.spinFor(forward, a * 360, deg, speed, velocityUnits::pct); */
    float kp = 0.07;
    float ki = 0.013;
    float error;
    
    float sum_error;
    float prev_error;
    do
    {
        float wheel_posisition = (LF.position(vex::deg) + RF.position(vex::deg)) / 2;
        printf("wheel_posistion: %f\n", wheel_posisition);

        
        
        
        error = targetRotation - wheel_posisition;
        
         printf("error: %f\n", error);
        sum_error = prev_error + error;
        float output = kp * error + ki * sum_error; 
        printf("output: %f\n", output);
        if(output > speed) output = speed;
        if(output < -speed) output = -speed;
        spin_motorsF(output);   
        prev_error = error;
    } while (fabs(error) > 2);
    stop_motors();

}
void turn(float degrees)
{
    //PID values
    float kp = 0.5;
    float ki = 0.01;
    //  flaot kd;

    //other
    float inertial_average;
    float error;
    float sum_error;  
    float prev_error;

    Inertial1.resetRotation();
    Inertial2.resetRotation();

    
    do{
        float inertial_average = (Inertial2.rotation(vex::deg) + Inertial2.rotation(vex::deg)) / 2;
        printf("inertial 1 = %f\n", Inertial1.rotation(vex::deg));
        printf("inertial 2 = %f\n", Inertial2.rotation(vex::deg));
        //printf("inertial_average = %f\n", inertial_average);
        error = degrees - inertial_average;
        //printf("error = %f\n", error);
        sum_error = error + prev_error;
        float output = error * kp + sum_error * ki;
        if(output>30) output = 30;
        if(output<-30) output = -30;
        spin_motorsT(output);
         prev_error = error;

    }while (fabs(error) > 1);
    stop_motors();
}
void turn_heading(float degrees)
{
    double kp = 1;
    float ki;
    //  flaot kd;
    float inertial_Haverage = ((Inertial1.heading() + Inertial2.heading())/2);
    float error = degrees;
    float sum_error;
    float prev_error;

    while (fabs(error) > 1)
    {
        error = degrees - inertial_Haverage;
        sum_error = prev_error + error;
        float output = error * kp + sum_error * ki;
        spin_motorsT(output);
        prev_error = error;
    }
    stop_motors();
}
void arcTurn(float radius, float angle, bool direction, float speed){
    float Kp = 0.1;
    float Ki = 0.01;

    float degPerRotation = (360 / (M_PI * 3.25)) * (3.0 / 4.0);
    printf("deg per rotation: %f\n", degPerRotation);
    
    LF.resetPosition();
    RF.resetPosition();

    float ArcLength = (2 * M_PI * radius) * (angle/360); 
    float leftDistance;
    float rightDistance;
    if(direction){//left
       float leftDistance = (2 * M_PI * (radius - 6)) * (angle/360); 
       float rightDistance = (2 * M_PI * (radius + 6)) * (angle/360);
    }
    if(!direction){//right
        float leftDistance = (2 * M_PI * (radius + 6)) * (angle/360); 
        float rightDistance = (2 * M_PI * (radius - 6)) * (angle/360);
    }
    float leftError;
    float rightError;
    do{
       float leftError = leftDistance - LF.position(vex::deg);
       float rightError = rightDistance - RF.position(vex::deg);
       float leftOutput = leftError * Kp;
       float rightOutput = rightError * Kp;

       if(leftOutput > speed) leftOutput = speed;
       if(leftOutput < -speed) leftOutput = -speed;
       if(rightOutput > speed) leftOutput = speed;
       if(rightOutput < -speed) rightOutput = -speed;
       
       leftSide(leftOutput);
       rightSide(rightOutput);
    }while(fabs(leftError) && fabs(rightError));

    // float degPerRotation = (360 / (M_PI * 3.25)) * (3.0 / 4.0);
    // printf("deg per rotation: %f\n", degPerRotation);
    // float targetRotation = distance * degPerRotation;
    // printf("target rotation: %f\n", targetRotation);
    // LF.resetPosition();
    // RF.resetPosition();
    // /*
    // LF.spinFor(forward, a * 360, deg, speed, velocityUnits::pct);
    // LM.spinFor(forward, a * 360, deg, speed, velocityUnits::pct);
    // LR.spinFor(forward, a * 360, deg, speed, velocityUnits::pct);
    // RF.spinFor(forward, a * 360, deg, speed, velocityUnits::pct);
    // RM.spinFor(forward, a * 360, deg, speed, velocityUnits::pct);
    // RF.spinFor(forward, a * 360, deg, speed, velocityUnits::pct); */
    // float kp = 0.12;
    // // float ki = 0.01;
    // float error;
    // // float sum_error;
    // // float prev_error;
    // do
    // {
    //     float wheel_posisition = (LF.position(vex::deg) + RF.position(vex::deg)) / 2;
    //     printf("wheel_posistion: %f\n", wheel_posisition);
    //     error = targetRotation - wheel_posisition;
    //      printf("error: %f\n", error);
    //     // sum_error = prev_error + error;
    //     float output = kp * error; //+ ki * sum_error;
    //     if(output > speed) output = speed;
    //     if(output < -speed) output = -speed;
    //     leftSide(output * left);  
    //     rightSide(output * right); 
    //     // prev_error = error;
    // } while (fabs(error) > 1.7);
    // stop_motors();
    
    

}
void intakes(float time){
  LI.spin(reverse, 100, pct);
  UI.spin(reverse, 75, pct);
  wait(time, sec);
  LI.stop();
  UI.stop();

}