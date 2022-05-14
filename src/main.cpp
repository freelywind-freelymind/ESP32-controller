#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#define butt1_pin 35
#define butt2_pin 32
#define butt3_pin 21
#define butt4_pin 19

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t upperAddress[] = {0x58, 0xBF, 0x25, 0x18, 0xB4, 0x84};
uint8_t chassisAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct struct_message {
  int butt_no;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo_upper;
esp_now_peer_info_t peerInfo_chassis;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
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
  
  // Register peer
  memcpy(peerInfo_upper.peer_addr, upperAddress, 6);
  peerInfo_upper.channel = 0;  
  peerInfo_upper.encrypt = false;

  memcpy(peerInfo_chassis.peer_addr, chassisAddress, 6);
  peerInfo_chassis.channel = 1;  
  peerInfo_chassis.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo_upper) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  if (esp_now_add_peer(&peerInfo_chassis) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(butt1_pin, INPUT_PULLDOWN);
  pinMode(butt2_pin, INPUT_PULLDOWN);
  pinMode(butt3_pin, INPUT_PULLDOWN);
  pinMode(butt4_pin, INPUT_PULLDOWN);

  attachInterrupt(digitalPinToInterrupt(butt1_pin), butt1_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(butt2_pin), butt2_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(butt3_pin), butt3_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(butt4_pin), butt4_interrupt, RISING);
}

void loop() {
  delay(1);
}

void butt1_interrupt(){
  myData.butt_no = '1';
  Serial.printf("Button %d had been pressed.", myData.butt_no);

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(upperAddress, (uint8_t *) &myData, sizeof(myData));
    
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}

void butt2_interrupt(){
  myData.butt_no = '2';
  Serial.printf("Button %d had been pressed.", myData.butt_no);

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(upperAddress, (uint8_t *) &myData, sizeof(myData));
    
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}

void butt3_interrupt(){
  myData.butt_no = '3';
  Serial.printf("Button %d had been pressed.", myData.butt_no);

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(upperAddress, (uint8_t *) &myData, sizeof(myData));
    
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}

void butt4_interrupt(){
  myData.butt_no = '4';
  Serial.printf("Button %d had been pressed.", myData.butt_no);

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(upperAddress, (uint8_t *) &myData, sizeof(myData));
    
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}