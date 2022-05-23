#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <CAN.h>

const int rx = 18;
const int tx = 19;
const long baudrate = 1000000;
const int package_id = 0x15;
const int package_size = 6;

typedef struct chassis_msg {
  int16_t x_axis;
  int16_t y_axis;
  int16_t w_axis;
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
  Serial.begin(9600);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //print its MAC address
  Serial.print("The MAC address is ");
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    //Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  CAN.setPins(rx, tx);
  // start the CAN bus at 500 kbps
  if (!CAN.begin(baudrate)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}
 
void loop() {
  if(flag){
    /*Serial.print("Button: ");
    Serial.print(chassis_order.butt_no);
    Serial.println();*/

    CAN.beginPacket(0x12);
    CAN.write(chassis_order.x_axis >> 8);
    CAN.write(chassis_order.x_axis & 0xFF);
    CAN.write(chassis_order.y_axis >> 8);
    CAN.write(chassis_order.y_axis & 0xFF);
    CAN.write(chassis_order.w_axis >> 8);
    CAN.write(chassis_order.w_axis & 0xFF);
    //send the package
    CAN.endPacket();    

    flag = false;
  }

  vTaskDelay(1);
}