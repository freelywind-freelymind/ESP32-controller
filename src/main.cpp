#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

typedef struct chassis_msg {
  float x_axis;
  float y_axis;
  float w_axis;
} chassis_msg;

chassis_msg chassis_order;

volatile bool flag = false;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&chassis_order, incomingData, sizeof(chassis_order));
  flag = true;
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    //Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  if(flag){
    /*Serial.print("Button: ");
    Serial.print(chassis_order.butt_no);
    Serial.println();*/
    Serial.print(chassis_order.butt_no);

    flag = false;
  }

  delay(1);
}