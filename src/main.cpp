#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <CAN.h>

const int rx = 18;
const int tx = 19;
const long baudrate = 1000000;
const int package_id = 0x12;
const int package_size = 1;

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
  Serial.begin(9600);
  
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
    Serial.print(upper_order.butt_no);
    Serial.println();*/

    CAN.beginPacket(package_id, 1);
    CAN.write(upper_order.butt_no);
    //send the package
    CAN.endPacket();

    flag = false;
  }

  vTaskDelay(1);
}