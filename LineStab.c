#include <kipr/botball.h>
#include <math.h>
#include <stdbool.h>

/*
    double abs(double x); function is required to run LineStab
    (absoulute value)
*/

/* ----------SETTING---------- */

//  Choose chasis (uncomment 1 line)
//#define CREATE
//#define LEGO
//-------------------------------
// Specify sensors if CREATE chasis is used, comment for default(0, 4000)
#ifdef CREATE
    //#define CliffMinReading 700        // Set minimum reading value for both sensors
    //#define CliffMaxReading 3000     // Set maximum reading value for both sensors
#endif

//--------------------------------------------
// Specify sensors and motors if LEGO chasis is used
#define LeftMotor 0
#define RightMotor 1

#define LeftTopHatPort 0
#define LeftTopHatMinReading 100
#define LeftTopHatMaxReading 4000

#define RightTopHatPort 1
#define RightTopHatMinReading 100
#define RightTopHatMaxReading 4000
//-------------------------------

// Here you can set custom speed limits,  comment for default(30, 150 for CREATE and 150, 1500 for LEGO)
//#define minSpeed 150
//#define maxSpeed 1500
//-------------------------------

// Time treshold in miliseconds (time after which LineStab will terminate if line is not detected), comment for default(3000 msec)
//#define TRESHOLD 3000
//-------------------------------

// Level of precision, comment for default(25). Default value is recommended
//#define LevelOfPrecision 25













int map(int x, int in_min, int in_max, int out_min, int out_max) {
// Rescale value from one range to another
return ( x - in_min ) * (out_max - out_min) / ( in_max - in_min ) + out_min;
} 

// direction defines
#ifndef forward
    #define forward 1
#endif
#ifndef backward
    #define backward 0
#endif

// default cliff sensors reading
#ifndef CliffMinReading
    #define CliffMinReading 700
#endif
#ifndef CliffMaxReading
    #define CliffMaxReading 3000
#endif

// default minimum speed
#ifndef minSpeed
    #ifdef CREATE
        #define minSpeed 30
    #endif
    #ifdef LEGO
        #define minSpeed 150
    #endif
#endif

// default maximum speed
#ifndef maxSpeed
    #ifdef CREATE 
        #define maxSpeed 150
    #endif
    #ifdef  LEGO
        #define maxSpeed 1500
    #endif
#endif

// Level of precision
#ifndef LevelOfPrecision
    #define LevelOfPrecision 25
#endif

// default treshold
#ifndef TRESHOLD
    #define TRESHOLD 3000
#endif

// set chasis
#ifdef CREATE
    #define Chasis 1
#endif
#ifdef LEGO
    #define Chasis 0
#endif

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
    int speedL = maxSpeed;
    speedL *= direction==true ? 1 : -1;
    int speedR = speedL;
    
    int t = systime();
        while(i<LevelOfPrecision) {
            
            if(Chasis) { 
                LEFT = map(get_create_lfcliff_amt(), CliffMaxReading, CliffMinReading, speedL, -speedL);
                RIGHT = map(get_create_rfcliff_amt(), CliffMaxReading, CliffMinReading, speedR, -speedR);
                create_drive_direct(LEFT, RIGHT);
            } else {
                LEFT = map(analog(LeftTopHatPort), LeftTopHatMinReading, LeftTopHatMaxReading, speedL, -speedL);
                RIGHT = map(analog(RightTopHatPort), RightTopHatMinReading, RightTopHatMaxReading, speedR, -speedR);
                mav(LeftMotor, LEFT);		
                mav(RightMotor, RIGHT);	
            }
            
            if(abs(LEFT) < abs(speedL*0.5)) L = true;			// check if sensors
            if(abs(RIGHT) < abs(speedR*0.5)) R = true;			// are near black tape
            
            if(direction && speedL > minSpeed && L) speedL -= maxSpeed/100;		// slowing down calibrating
            if(!direction && speedL < -minSpeed && L) speedL += maxSpeed/100;		// on left TopHat
            
            if(direction && speedR > minSpeed && R) speedR -= maxSpeed/100;		// slowing down calibrating
            if(!direction && speedR < -minSpeed && R) speedR += maxSpeed/100;		// on right TopHat     
            
            if(!direction && speedL > -minSpeed) speedL = -minSpeed;	// min speed border
            if(!direction && speedR > -minSpeed) speedR = -minSpeed;	
            
            if(direction && speedL < minSpeed) speedL = minSpeed;		// max speed border
            if(direction && speedR < minSpeed) speedR = minSpeed;
            if(abs(speedL) <= minSpeed && abs(speedR) <= minSpeed && abs(RIGHT) < minSpeed && abs(LEFT) < minSpeed) i++;	// both sensors calibrated
            if(!L && !R && systime()-t > TRESHOLD) {				// Line not found, disable Line Stab
             	return -1;   
            }
        }
  	if(Chasis) {
        create_stop();   
    } else {
        freeze(LeftMotor);
        freeze(RightMotor);
    }
    return 0;
}
