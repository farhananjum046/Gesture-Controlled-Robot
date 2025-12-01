#include <WiFi.h>
#include <esp_now.h>

// Motor driver pins
const int motor1Pin1 = 5;
const int motor1Pin2 = 18;
const int motor2Pin1 = 19;
const int motor2Pin2 = 23;

typedef struct struct_message {
  char command[10];
} struct_message;

struct_message incomingData;

void moveForward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void moveBackward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void moveLeft() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void moveRight() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}

void onDataReceived(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));
  Serial.print("🛰️ Command Received: ");
  Serial.println(incomingData.command);

  if (strcmp(incomingData.command, "forward") == 0) moveForward();
  else if (strcmp(incomingData.command, "backward") == 0) moveBackward();
  else if (strcmp(incomingData.command, "left") == 0) moveLeft();
  else if (strcmp(incomingData.command, "right") == 0) moveRight();
  else stopMotors();
}

void setup() {
  Serial.begin(115200);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  stopMotors();

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(onDataReceived);
  Serial.println("🚗 Receiver Ready");
}

void loop() {
  // Nothing here
}
