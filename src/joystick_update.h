#include <Arduino.h>
#include "pinmap.h"

int axis[4];
volatile float softening = 0.2;
volatile float max_speed = 1;
volatile float max_rotation_speed = 1;
int deadband = 110; 

float mapfloat(long x, float in_min, float in_max, float out_min, float out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

float soften_value(float axis, float soften, float max_v){
  return axis * pow(abs(axis/max_v), soften);
}

void joystick_update(){
  int temp = 0;

  temp = analogRead(Ljoystick_x_pin);
  if(temp < 0) {
    axis[0] = -(temp) - 2046;
  }
  else{
    axis[0] = 2047 - temp;
  }

  temp = analogRead(Ljoystick_y_pin);
  if(temp < 0) {
    axis[1] = -(temp) - 2046;
  }
  else{
    axis[1] = 2047 - temp;
  }

  temp = analogRead(Rjoystick_x_pin);
  if(temp < 0) {
    axis[2] = -(temp) - 2046;
  }
  else{
    axis[2] = 2047 - temp;
  }

  temp = analogRead(Rjoystick_y_pin);
  if(temp < 0) {
    axis[3] = -(temp) - 2046;
  }
  else{
    axis[3] = 2047 - temp;
  }
}