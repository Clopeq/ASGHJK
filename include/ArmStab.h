#include <kipr/botball.h>
#include "ServoFuns.h"

void ArmStab() {
	set_servo_position(1,600);
    set_servo_position(2,1100);
   	set_servo_position(3,1100);
    
   	msleep(500);
        
    ChangeServoPosInTime(1,300,1000);
    ChangeServoPosInTime(2,1300,2000);
    ChangeServoPosInTime(3,1850,1000);
}
