#include <kipr/botball.h>

void ChangeServoPosInTime(unsigned int servo_port, double finish_position, int mtime) {
	// changes servo position over time
	if(finish_position > 2046 || finish_position < 1 || servo_port > 3) printf("ChangeServoPosInTime - error!\n");
	double current_position = get_servo_position(servo_port);
	double change = (finish_position - current_position) / mtime * 10;
	while(change > 0 ? get_servo_position(servo_port) < finish_position : get_servo_position(servo_port) > finish_position) {
		current_position += change;
		set_servo_position(servo_port, current_position);
		msleep(10);
	}
	set_servo_position(servo_port, finish_position);
}