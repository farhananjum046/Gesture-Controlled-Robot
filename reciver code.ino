#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

uint8_t receiverAddress[] = {0x1C, 0x69, 0x20, 0xA2, 0xC4, 0x80}; // Replace with your RECEIVER MAC

typedef struct struct_message {
  char command[10];
} struct_message;

struct_message msg;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Transmitter ready");
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  if (ay > 8000) {
    strcpy(msg.command, "forward");
  } else if (ay < -8000) {
    strcpy(msg.command, "backward");
  } else if (ax > 8000) {
    strcpy(msg.command, "left");
  } else if (ax < -8000) {
    strcpy(msg.command, "right");
  } else {
    strcpy(msg.command, "stop");
  }

  esp_now_send(receiverAddress, (uint8_t *)&msg, sizeof(msg));
  delay(200);
}