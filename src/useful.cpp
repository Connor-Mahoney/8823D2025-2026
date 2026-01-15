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
extern rotation leftRotation;
extern rotation rightRotation;
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
    printf("STARTFYFKJMF\n");
    float targetDegrees = (inches/(3.25*M_PI))*360;
    printf("targetDegrees =%f\n", targetDegrees);
    float posistion;
    float output;
    //  float targetRotation = inches * degPerRotation;
    // printf("target rotation: %f\n", targetRotation);
    
    float kp = 0.06;
    float ki = 0.02;
    float kd;
    float error;
    float sum_error;
    float prev_error;
    float dif_error;

    rightRotation.resetPosition();
    leftRotation.resetPosition();

    do
    {
        float wheelAverage = ((-1*rightRotation.position(vex::deg)) + leftRotation.position(vex::deg)) / 2;
        printf("targetDegrees =%f\n", targetDegrees);
        printf("right = %f\n", -1*rightRotation.position(vex::deg));
        printf("left = %f\n", leftRotation.position(vex::deg));
        // printf("wheel_posistion: %f\n", wheel_posisition);

        
        
        
         error = targetDegrees - wheelAverage;
        printf("error = %f\n", error);
        sum_error = (prev_error + error);
        output = kp * error + ki * sum_error; 
        // printf("output: %f\n", output);
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
        float inertial_average = (Inertial1.rotation(vex::deg) + Inertial2.rotation(vex::deg)) / 2;
        printf("inertial 1 = %f\n", Inertial1.rotation(vex::deg));
        printf("inertial 2 = %f\n", Inertial2.rotation(vex::deg));
        //printf("inertial_average = %f\n", inertial_average);
        error = degrees - inertial_average;
        printf("error = %f\n", error);
        sum_error = error + prev_error;
        float output = error * kp + sum_error * ki;
        if(output>30) output = 30;
        if(output<-30) output = -30;
        spin_motorsT(output);
         prev_error = error;

    }while (fabs(error) > 1);
    stop_motors();
}
void turnHeading(float degrees)
{
    double kp = 1;
    float ki;
    //  flaot kd;
    float inertialH;
    float error = degrees;
    float sum_error;
    float prev_error;

     do{
    
        error = degrees - Inertial1.rotation(vex::deg);
        printf("error = %f\n", error);
        sum_error = error + prev_error;
        float output = error * kp; //+ sum_error * ki;
        if(output>30) output = 30;
        if(output<-30) output = -30;
        spin_motorsT(output);
         prev_error = error;

    }while (fabs(error) > 1);
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
    }while(fabs(leftError) > 1 && fabs(rightError) > 1);
    

}
void intakes(float time){
  LI.spin(reverse, 100, pct);
  UI.spin(reverse, 75, pct);
  wait(time, sec);
  LI.stop();
  UI.stop();

}