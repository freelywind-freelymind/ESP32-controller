#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

typedef struct upper_msg {
  char butt_no;
} upper_msg;

upper_msg upper_order;

volatile bool flag = false;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&upper_order, incomingData, sizeof(upper_order));

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
    Serial.print(upper_order.butt_no);
    Serial.println();*/

    Serial.print(upper_order.butt_no);

    flag = false;
  }

  vTaskDelay(1);
}