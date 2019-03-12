#include <kipr/botball.h>
#include "DriveFuns.h"

void DrivewayForLine() {
    DriveLine(-1500,-1500,FrontR,Black);
    DriveLine(-1500,-1500,FrontR,White);
    LineStab(Front);
    Drive(-1500,-1500,100);
    Drive(1500,-1500,280);
    Drive(-1500,-1500,1000);
    DriveLine(-1500,-1500,FrontL,Black);
    Drive(0,1500,300);
    DriveLine(0,1500,FrontL,White);
}