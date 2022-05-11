#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#define butt1_pin 35
#define butt2_pin 32
#define butt3_pin 21
#define butt4_pin 19

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct struct_message {
  int butt_no;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

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
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(butt1_pin, INPUT);
  pinMode(butt2_pin, INPUT);
  pinMode(butt3_pin, INPUT);
  pinMode(butt4_pin, INPUT);
}

void loop() {
  bool flag = false;

  if(digitalRead(butt1_pin)){
    myData.butt_no = 1;
    flag =true;
  } else if(digitalRead(butt2_pin)){
    myData.butt_no = 2;
    flag =true;
  } else if(digitalRead(butt3_pin)){
    myData.butt_no = 3;
    flag =true;
  } else if(digitalRead(butt4_pin)){
    myData.butt_no = 4;
    flag =true;
  }

  if(flag){
    Serial.printf("Button %d had been pressed.", myData.butt_no);

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
  }

  //update freq 1000Hz
  delay(1);
}