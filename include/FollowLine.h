/*#define FollowLine(speedl,speedr,sensor,argument,function) {\
	while(argument) { \
    	if(analog(sensor.port) < sensor.avg) Drive(speedl,speedr,0); \
    	else Drive(speedr,speedl,0); \
         function;	\
    }}           

;*/