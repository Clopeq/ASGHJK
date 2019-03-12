#include <kipr/botball.h>
#include "DriveFuns.h"
#include "ArmStab.h"
#include "DrivewayForLine.h"

int main()
{
    enable_servos();
    
    ArmStab();
    
    long overall = systime();
    
    int speed = 1;
    
    while(systime() - overall < 2500) {
    	if(analog(FrontL.port) < FrontL.avg) {
        	mav(MotorL, 600*speed);
            mav(MotorR, 400*speed);
        } else {
            mav(MotorR, 600*speed);
            mav(MotorL, 400*speed);
        }
    }
    Stop(1);
    ChangeServoPosInTime(3, 350, 400);
    ChangeServoPosInTime(2, 500, 750);
    Drive(300, -300, 300);
    Drive(-300, 300, 300);
    set_servo_position(2, 1700); msleep(100);
    set_servo_position(3, 1800);
    
    overall = systime();
    
    while(systime() - overall < 2500) {
    	if(analog(FrontL.port) < FrontL.avg) {
        	mav(MotorL, 600*speed);
            mav(MotorR, 400*speed);
        } else {
            mav(MotorR, 600*speed);
            mav(MotorL, 400*speed);
        }
    }
    Stop(1);
    ChangeServoPosInTime(3, 350, 400);
    ChangeServoPosInTime(2, 500, 750);
    Drive(300, -300, 300);
    Drive(-300, 300, 300);
    set_servo_position(2, 1700); msleep(100);
    set_servo_position(3, 1800);
    
    Stop(1);
    
    /*
    long overall = systime();
    
    DrivewayForLine();
    
    FollowLineTime(1500,700,1,1000);
    FollowLineTime(700,600,1,1000);
    ChangeServoPosInTime(3,55,1000);
    FollowLineTime(700,600,1,700);
    ChangeServoPosInTime(2,500,2000);
    ChangeServoPosInTime(2,1300,2000);
    set_servo_position(3,1850);
    FollowLineTime(1500,1300,1,2000);
    
    msleep(3000);
    
    printf("overall = %ld\n", systime() - overall);
    */
    disable_servos();
        
    //thread chuj = thread_create(Scan);
    //thread_destroy(chuj);
        
    return 0;
}
