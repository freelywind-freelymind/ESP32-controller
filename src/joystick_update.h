#include <Arduino.h>
#include "pinmap.h"
#include "buttCode.h"

typedef struct upper_msg {
  char butt_no;
} upper_msg;

upper_msg upper_order;

int axis[4];
int16_t X_axis;
int16_t Y_axis;
int16_t W_axis;

float softening = 0.2;
float max_speed = 1.0;
float max_rotation_speed = 1.0;
int deadband = 110;

bool flag = false;
bool* butt_flag = new bool[14]{false,false,false,false,false,false,false,
                              false,false,false,false,false,false,false};

void init_pinout(){
  pinMode(L1_pin, INPUT_PULLDOWN);
  pinMode(L2_pin, INPUT_PULLDOWN);
  pinMode(up_pin, INPUT_PULLDOWN);
  pinMode(down_pin, INPUT_PULLDOWN);
  pinMode(left_pin, INPUT_PULLDOWN);
  pinMode(right_pin, INPUT_PULLDOWN);

  pinMode(R1_pin, INPUT_PULLDOWN);
  pinMode(R2_pin, INPUT_PULLDOWN);
  pinMode(triangle_pin, INPUT_PULLDOWN);
  pinMode(circle_pin, INPUT_PULLDOWN);
  pinMode(square_pin, INPUT_PULLDOWN);
  pinMode(cross_pin, INPUT_PULLDOWN);

  pinMode(question_pin, INPUT_PULLDOWN);
  pinMode(estop_pin, INPUT_PULLDOWN);

  pinMode(Ljoystick_x_pin, INPUT);
  pinMode(Ljoystick_y_pin, INPUT);
  pinMode(Rjoystick_x_pin, INPUT);
  pinMode(Rjoystick_y_pin, INPUT);
}

void L1_interrupt(){
  upper_order.butt_no = L1;
  flag = true;
}
void L2_interrupt(){
  upper_order.butt_no = L2;
  flag = true;
}
void up_interrupt(){
  upper_order.butt_no = up;

  if (max_speed <= 3.0) {
    max_speed += 0.5;
  }
  
  flag = true;
}
void left_interrupt(){
  upper_order.butt_no = left;

  if (softening >= 0.05){
    softening -= 0.2;
  }

  flag = true;
}
void down_interrupt(){
  upper_order.butt_no = down;

  if (max_speed >= 1.0) {
    max_speed -= 0.5;
  }

  flag = true;
}
void right_interrupt(){
  upper_order.butt_no = right;

  if (softening <= 0.8){
    softening += 0.2;
  }

  flag = true;
}

void R1_interrupt(){
  upper_order.butt_no = R1;
  flag = true;
}
void R2_interrupt(){
  upper_order.butt_no = R2;
  flag = true;
}
void triangle_interrupt(){
  upper_order.butt_no = triangle;
  flag = true;
}
void circle_interrupt(){
  upper_order.butt_no = circle;
  flag = true;
}
void square_interrupt(){
  upper_order.butt_no = square;
  flag = true;
}
void cross_interrupt(){
  upper_order.butt_no = cross;
  flag = true;
}

void question_interrupt(){
  upper_order.butt_no = question;
  flag = true;
}
void estop_interrupt(){
  upper_order.butt_no = estop;
  flag = true;
}

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

  //debouncing
  if(digitalRead(up_pin)){
    if(!butt_flag[0]){
      butt_flag[0] = !butt_flag[0];
      up_interrupt();
    }   
  }
  else{
    butt_flag[0] = false;
  }
  
  //debouncing
  if(digitalRead(down_pin)){
    if(!butt_flag[1]){
      butt_flag[1] = !butt_flag[1];
      down_interrupt();
    }
  }
  else{
    butt_flag[1] = false;
  }

  //debouncing
  if(digitalRead(left_pin)){
    if(!butt_flag[2]){
      butt_flag[2] = !butt_flag[2];
      left_interrupt();
    }
  }
  else{
    butt_flag[2] = false;
  }

  //debouncing
  if(digitalRead(right_pin)){
    if(!butt_flag[3]){
      butt_flag[3] = !butt_flag[3];
      right_interrupt();
    }
  }
  else{
    butt_flag[3] = false;
  }

  //debouncing
  if(digitalRead(L1_pin)){
    if(!butt_flag[4]){
      butt_flag[4] = !butt_flag[4];
      L1_interrupt();
    }   
  }
  else{
    butt_flag[4] = false;
  }

  //debouncing
  if(digitalRead(L2_pin)){
    if(!butt_flag[5]){
      butt_flag[5] = !butt_flag[5];
      L2_interrupt();
    }   
  }
  else{
    butt_flag[5] = false;
  }

  //debouncing
  if(digitalRead(R1_pin)){
    if(!butt_flag[6]){
      butt_flag[6] = !butt_flag[6];
      R1_interrupt();
    }   
  }
  else{
    butt_flag[6] = false;
  }

  //debouncing
  if(digitalRead(R2_pin)){
    if(!butt_flag[7]){
      butt_flag[7] = !butt_flag[7];
      R2_interrupt();
    }   
  }
  else{
    butt_flag[7] = false;
  }

  //debouncing
  if(digitalRead(triangle_pin)){
    if(!butt_flag[8]){
      butt_flag[8] = !butt_flag[8];
      triangle_interrupt();
    }   
  }
  else{
    butt_flag[8] = false;
  }

  //debouncing
  if(digitalRead(circle_pin)){
    if(!butt_flag[9]){
      butt_flag[9] = !butt_flag[9];
      circle_interrupt();
    }   
  }
  else{
    butt_flag[9] = false;
  }

  //debouncing
  if(digitalRead(square_pin)){
    if(!butt_flag[10]){
      butt_flag[10] = !butt_flag[10];
      square_interrupt();
    }   
  }
  else{
    butt_flag[10] = false;
  }

  //debouncing
  if(digitalRead(cross_pin)){
    if(!butt_flag[11]){
      butt_flag[11] = !butt_flag[11];
      cross_interrupt();
    }   
  }
  else{
    butt_flag[11] = false;
  }

  //debouncing
  if(digitalRead(question_pin)){
    if(!butt_flag[12]){
      butt_flag[12] = !butt_flag[12];
      question_interrupt();
    }   
  }
  else{
    butt_flag[12] = false;
  }

  //debouncing
  if(digitalRead(estop_pin)){
    if(!butt_flag[13]){
      butt_flag[13] = !butt_flag[13];
      estop_interrupt();
    }   
  }
  else{
    butt_flag[13] = false;
  }
}