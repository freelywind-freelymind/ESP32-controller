#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "pinmap.h"
#include "joystick_update.h"

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t upperAddress[] = {0x94, 0xB9, 0x7E, 0xFA, 0x4D, 0x00};
uint8_t chassisAddress[] = {0x58, 0xBF, 0x25, 0x18, 0xB4, 0x84};

esp_now_peer_info_t peerInfo;

// callback when data is sent
//Serial in the interrupt may leading the hang
/*void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}*/

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("The MAC address is ");
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //for debug only
  //enable it may leading the hang/bug
  //esp_now_register_send_cb(OnDataSent);
  
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Register peer
  memcpy(peerInfo.peer_addr, upperAddress, 6);
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  memcpy(peerInfo.peer_addr, chassisAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  init_pinout();
}

void loop() {
  joystick_update();

  X_axis = -(int16_t)(soften_value(mapfloat(axis[3], -2048, 2047, -max_speed, max_speed),softening, max_speed) * 1000);
  Y_axis = (int16_t)(soften_value(mapfloat(axis[2], -2048, 2047, -max_speed, max_speed),softening, max_speed) * 1000);
  W_axis = -(int16_t)(soften_value(mapfloat(axis[1], -2048, 2047, -max_rotation_speed, max_rotation_speed),softening, max_rotation_speed) * 1000);

  if (abs(X_axis) < deadband){
    X_axis = 0;
  }
  if (abs(Y_axis) < deadband){
    Y_axis = 0;
  }
  if (abs(W_axis) < deadband){
    W_axis = 0;
  }

  if(estop_flag){
    X_axis = 0;
    Y_axis = 0;
    W_axis = 0;
    msg.butt_no = estop;
  }

  //Serial.printf("Button %c had been pressed.\n", msg.butt_no);
  /*Serial.print("X:");
  Serial.print(X_axis);
  Serial.print(" Y:");
  Serial.print(Y_axis);
  Serial.print(" W:");
  Serial.print(W_axis);
  Serial.println("");*/

  //update the msg of package
  msg.x_axis = X_axis;
  msg.y_axis = Y_axis;
  msg.w_axis = W_axis;
  // Send message via ESP-NOW
  esp_now_send(0, (uint8_t *) &msg, sizeof(msg));

  //update freq 100Hz
  vTaskDelay(10);
}