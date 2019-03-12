int map(int x, int in_min, int in_max, int out_min, int out_max) {
// Rescale value from one range to another
return ( x - in_min ) * (out_max - out_min) / ( in_max - in_min ) + out_min;
} 