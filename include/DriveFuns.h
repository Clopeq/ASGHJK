#ifndef DriveFuns_H_
#define DriveFuns_H_

#include <kipr/botball.h>
#include "Math.h"
#include "Constants.h"
#include "FollowLine.h"

//Funs for drive
void Drive(int SpeedL, int SpeedR, int time) {
    mav(MotorL,SpeedL);
    mav(MotorR,SpeedR);
    msleep(time);
    Stop(1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
//Funs for drive for line
void DriveLine(int SpeedL, int SpeedR, Tophat sensor, int Colour) {
    if (Colour == 1){
    mav(MotorL,SpeedL);
    mav(MotorR,SpeedR);
    while(analog(sensor.port) < sensor.avg);
    } else {
    mav(MotorL,SpeedL);
    mav(MotorR,SpeedR);
    while(analog(sensor.port) > sensor.avg);
    }
    Stop(1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Follow line
#define FollowLine(speedl,speedr,sensor,argument,function) {\
	while(argument) { \
    	if(analog(sensor.port) < sensor.avg) Drive(speedl,speedr,0); \
    	else Drive(speedr,speedl,0); \
         function;	\
    }}           

;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Follow line for time
void FollowLineTime(int speedL, int speedR, int port1, int time) {
    int t = systime();
 while(systime() - t < time){
        if(analog(port1) < 1000) {
             mav(1,speedR);
             mav(0,speedL);    
        } else {
             mav(1,speedL);
             mav(0,speedR);
        }
    }
    Stop(1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Line stabilization
#define minSpeed 150
#define maxSpeed 1500

int LineStab(bool direction) {
    /*
    	return value:
        		0 - success
                -1 - fail
    
    	Calibrate to Line
        
        direction -	direction you want to move when robot is not on line
        		forward = true
       			backward = false
    */
    int i = 0;
    bool L = false, R = false;
    int LEFT, RIGHT;
    int speedL = 1500;
    speedL *= direction==true ? 1 : -1;
    int speedR = speedL;
    
    int t = systime();
        while(i<25) {
            LEFT = map(analog(FrontL.port), FrontL.min, FrontL.max, speedL, -speedL);
            RIGHT = map(analog(FrontR.port), FrontR.min, FrontR.max, speedR, -speedR);
            
            mav(MotorL, LEFT);		
            mav(MotorR, RIGHT);		
            
            if(abs(LEFT) < abs(speedL*0.5)) L = true;			// check if sensors
            if(abs(RIGHT) < abs(speedR*0.5)) R = true;			// are near black tape
            
            if(direction && speedL > minSpeed && L) speedL -= 15;		// slowing down calibrating
            if(!direction && speedL < -minSpeed && L) speedL += 15;		// on left TopHat
            
            if(direction && speedR > minSpeed && R) speedR -= 15;		// slowing down calibrating
            if(!direction && speedR < -minSpeed && R) speedR += 15;		// on right TopHat     
            
            if(!direction && speedL > -minSpeed) speedL = -minSpeed;	// min speed border
            if(!direction && speedR > -minSpeed) speedR = -minSpeed;	
            
            if(direction && speedL < minSpeed) speedL = minSpeed;		// max speed border
            if(direction && speedR < minSpeed) speedR = minSpeed;
            if(abs(speedL) <= minSpeed && abs(speedR) <= minSpeed && abs(RIGHT) < minSpeed && abs(LEFT) < minSpeed) i++;	// both sensors calibrated
            if(!L && !R && systime()-t > 3000) {				// Line not found, disable Line Stab
             	return -1;   
            }
        }
  	Stop(1);
    return 0;
}

#endif