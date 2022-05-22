#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "pinmap.h"
#include "joystick_update.h"

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t upperAddress[] = {0x58, 0xBF, 0x25, 0x18, 0xB4, 0x84};
uint8_t chassisAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct chassis_msg {
  float x_axis;
  float y_axis;
  float w_axis;
} chassis_msg;

chassis_msg chassis_order;

esp_now_peer_info_t peerInfo_upper;
esp_now_peer_info_t peerInfo_chassis;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  Serial.print("The MAC address is ");
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // get the status of Trasnmitted packet
  //esp_now_register_send_cb(OnDataSent);
  
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

  init_pinout();
}

void loop() {
  joystick_update();

  if(flag){
    Serial.printf("Button %c had been pressed.\n", upper_order.butt_no);

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(upperAddress, (uint8_t *) &upper_order, sizeof(upper_order));

    flag = false;
  }

  X_axis = soften_value(mapfloat(axis[2], -2048, 2047, -max_speed, max_speed),softening, max_speed) * 1000;
  Y_axis = soften_value(mapfloat(axis[3], -2048, 2047, -max_speed, max_speed),softening, max_speed) * 1000;
  W_axis = soften_value(mapfloat(axis[0], -2048, 2047, -max_rotation_speed, max_rotation_speed),softening, max_rotation_speed) * 1000;

  if (abs(X_axis) < deadband){
    X_axis = 0;
  }
  if (abs(Y_axis) < deadband){
    Y_axis = 0;
  }
  if (abs(W_axis) < deadband){
    W_axis = 0;
  }

  /*Serial.print("X:");
  Serial.print(X_axis);
  Serial.print(" Y:");
  Serial.print(Y_axis);
  Serial.print(" W:");
  Serial.print(W_axis);
  Serial.println("");*/

  //update the msg of package
  chassis_order.x_axis = X_axis;
  chassis_order.y_axis = Y_axis;
  chassis_order.w_axis = W_axis;
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(chassisAddress, (uint8_t *) &chassis_order, sizeof(chassis_order));

  //update freq 100Hz
  vTaskDelay(10);
}