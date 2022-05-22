#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "pinmap.h"
#include "joystick_update.h"

float X_axis;
float Y_axis;
float W_axis;

volatile bool flag = false;
TaskHandle_t Task2;

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t upperAddress[] = {0x58, 0xBF, 0x25, 0x18, 0xB4, 0x84};
uint8_t chassisAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct upper_msg {
  char butt_no;
} upper_msg;

typedef struct chassis_msg {
  float x_axis;
  float y_axis;
  float w_axis;
} chassis_msg;

upper_msg upper_order;
chassis_msg chassis_order;

esp_now_peer_info_t peerInfo_upper;
esp_now_peer_info_t peerInfo_chassis;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void L1_interrupt(){
  upper_order.butt_no = '1';
  flag = true;
}
void L2_interrupt(){
  upper_order.butt_no = '2';
  flag = true;
}
void up_interrupt(){
  upper_order.butt_no = '3';

  if (max_speed <= 3.0) {
    max_speed += 0.5;
  }
  
  flag = true;
}
void left_interrupt(){
  upper_order.butt_no = '4';
  flag = true;
}
void down_interrupt(){
  upper_order.butt_no = '5';

  if (max_speed >= 1.0) {
    max_speed -= 0.5;
  }

  flag = true;
}
void right_interrupt(){
  upper_order.butt_no = '6';
  flag = true;
}

void R1_interrupt(){
  upper_order.butt_no = '7';
  flag = true;
}
void R2_interrupt(){
  upper_order.butt_no = '8';
  flag = true;
}
void triangle_interrupt(){
  upper_order.butt_no = '9';
  flag = true;
}
void circle_interrupt(){
  upper_order.butt_no = 'a';
  flag = true;
}
void square_interrupt(){
  upper_order.butt_no = 'b';
  flag = true;
}
void cross_interrupt(){
  upper_order.butt_no = 'c';
  flag = true;
}

void question_interrupt(){
  upper_order.butt_no = 'd';
  flag = true;
}
void estop_interrupt(){
  upper_order.butt_no = 'x';
  flag = true;
}

void Task2code( void * pvParameters ){
  pinMode(L1_pin, INPUT_PULLDOWN);
  pinMode(L2_pin, INPUT_PULLDOWN);
  pinMode(up_pin, INPUT_PULLDOWN);
  pinMode(left_pin, INPUT_PULLDOWN);
  pinMode(down_pin, INPUT_PULLDOWN);
  pinMode(right_pin, INPUT_PULLDOWN);

  pinMode(R1_pin, INPUT_PULLDOWN);
  pinMode(R2_pin, INPUT_PULLDOWN);
  pinMode(triangle_pin, INPUT_PULLDOWN);
  pinMode(circle_pin, INPUT_PULLDOWN);
  pinMode(square_pin, INPUT_PULLDOWN);
  pinMode(cross_pin, INPUT_PULLDOWN);

  pinMode(question_pin, INPUT_PULLDOWN);
  pinMode(estop_pin, INPUT_PULLDOWN);

  attachInterrupt(digitalPinToInterrupt(L1_pin), L1_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(L2_pin), L2_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(up_pin), up_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(left_pin), left_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(down_pin), down_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(right_pin), right_interrupt, RISING);

  attachInterrupt(digitalPinToInterrupt(R1_pin), R1_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(R2_pin), R2_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(triangle_pin), triangle_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(circle_pin), circle_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(square_pin), square_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(cross_pin), cross_interrupt, RISING);

  attachInterrupt(digitalPinToInterrupt(question_pin), question_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(estop_pin), estop_interrupt, RISING);

  while(true){
    if(flag){
      Serial.printf("Button %c had been pressed.", upper_order.butt_no);

      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(upperAddress, (uint8_t *) &upper_order, sizeof(upper_order));

      flag = false;
    }
    
    delay(1);
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  peerInfo_upper.channel = 0;  
  peerInfo_upper.encrypt = false;
  
  // Register peer
  memcpy(peerInfo_upper.peer_addr, upperAddress, 6);
  // Add peer        
  if (esp_now_add_peer(&peerInfo_upper) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  memcpy(peerInfo_chassis.peer_addr, chassisAddress, 6);
  if (esp_now_add_peer(&peerInfo_chassis) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  xTaskCreatePinnedToCore(
    Task2code,   /* Task function. */
    "Task2",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task2,      /* Task handle to keep track of created task */
    0);          /* pin task to core 1 */
  delay(500);

  pinMode(Ljoystick_x_pin, INPUT);
  pinMode(Ljoystick_y_pin, INPUT);
  pinMode(Rjoystick_x_pin, INPUT);
  pinMode(Rjoystick_y_pin, INPUT);
}

void loop() {

  joystick_update();

  if(digitalRead(up_pin)){
    up_interrupt();
  }
  
  if(digitalRead(down_pin)){
    down_interrupt();
  }

  noInterrupts();
  X_axis = soften_value(mapfloat(axis[2], -2048, 2047, -max_speed, max_speed),softening, max_speed) * 1000;
  Y_axis = soften_value(mapfloat(axis[3], -2048, 2047, -max_speed, max_speed),softening, max_speed) * 1000;
  W_axis = soften_value(mapfloat(axis[0], -2048, 2047, -max_rotation_speed, max_rotation_speed),softening, max_rotation_speed) * 1000;
  interrupts();

  if (abs(X_axis) < deadband){
    X_axis = 0;
  }
  if (abs(Y_axis) < deadband){
    Y_axis = 0;
  }
  if (abs(W_axis) < deadband){
    W_axis = 0;
  }

  Serial.print("X:");
  Serial.print(X_axis);
  Serial.print(" Y:");
  Serial.print(Y_axis);
  Serial.print(" W:");
  Serial.print(W_axis);
  Serial.println("");

  //update the msg of package
  chassis_order.x_axis = X_axis;
  chassis_order.y_axis = Y_axis;
  chassis_order.w_axis = W_axis;
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(chassisAddress, (uint8_t *) &chassis_order, sizeof(chassis_order));

  //update freq 100Hz
  delay(10);
}