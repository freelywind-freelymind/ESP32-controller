#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#define butt1_pin 32
#define butt2_pin 33
#define butt3_pin 25
#define butt4_pin 26
#define butt5_pin 23
#define butt6_pin 22
#define butt7_pin 21
#define butt8_pin 19

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t upperAddress[] = {0x58, 0xBF, 0x25, 0x18, 0xB4, 0x84};
uint8_t chassisAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct struct_message {
  char butt_no;
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

void butt5_interrupt(){
  myData.butt_no = '5';
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

void butt6_interrupt(){
  myData.butt_no = '6';
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

void butt7_interrupt(){
  myData.butt_no = '7';
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

void butt8_interrupt(){
  myData.butt_no = '8';
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

  pinMode(butt1_pin, INPUT_PULLUP);
  pinMode(butt2_pin, INPUT_PULLUP);
  pinMode(butt3_pin, INPUT_PULLUP);
  pinMode(butt4_pin, INPUT_PULLUP);
  pinMode(butt5_pin, INPUT_PULLUP);
  pinMode(butt6_pin, INPUT_PULLUP);
  pinMode(butt7_pin, INPUT_PULLUP);
  pinMode(butt8_pin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(butt1_pin), butt1_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(butt2_pin), butt2_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(butt3_pin), butt3_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(butt4_pin), butt4_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(butt5_pin), butt1_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(butt6_pin), butt2_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(butt7_pin), butt3_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(butt8_pin), butt4_interrupt, FALLING);
}

void loop() {
  delay(1);
}