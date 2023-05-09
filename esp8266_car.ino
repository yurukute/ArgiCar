#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

// FIRMWARE CONFIGURATION
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLwxz7Etgp"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "s3_jVWTCNlIVRDlTlMGAvxWOXxsJ84PT"

// Define pins
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define moistPin D5
#define echoPin  D6
#define trigPin  D7
#define servoPin D8
#define relayPin D0

// Define servo angles
#define UPPER 45
#define LOWER 0

// Joystick's axises
#define CENTER 50
int x = CENTER;
int y = CENTER;

// Define tank's height
#define MAXHEIGHT 6.60
int waterLevel = 100;
int waterDuration = 5;

Servo servo;

void setup() {
  Serial.begin(9600);

  // Set output pins
  int pins[] = {IN1, IN2, IN3, IN4, moistPin, relayPin};
  int n = sizeof(pins)/sizeof(int);
  for(int i = 0; i < n; i++)
    pinMode(pins[i], OUTPUT);

  // Default voltage for relay
  // Use HIGH on low-voltage switching relay
  digitalWrite(relayPin, HIGH);

  // Attach servo
  servo.attach(servoPin, 544, 2400);
  servo.write(UPPER);

  // Initialize the Blynk library
  char ssid[] = "B1906647"; //Enter your WIFI name
  char pass[] = "88888888"; //Enter your WIFI password
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
}

void loop() {
  Blynk.run();
  updateWaterLevel();
  smartCar();
  delay(100);
}

// Get Blynk values
BLYNK_WRITE(V0) {
  x = param[0].asInt();
}

BLYNK_WRITE(V1) {
  y = param[0].asInt();
}

// Water button
BLYNK_WRITE(V2) {
  if(param.asInt()){
    servo.write(LOWER);
    delay(1000);
    int moisture = readMoisture();
    Blynk.virtualWrite(V4, moisture);
    if (moisture <= 60){
      Blynk.virtualWrite(V3, "Watering");
      water();
      Blynk.virtualWrite(V3, "Watering: DONE");
      Blynk.virtualWrite(V4, readMoisture());
    }
    else Blynk.virtualWrite(V3, "Not watering");
    servo.write(UPPER);
    Blynk.virtualWrite(V2, LOW);
  }
}

// Water slider
BLYNK_WRITE(V5){
  waterDuration = param[0].asInt();
}

void sendNotification(String message){
  Serial.println("Notification sent!" + message);
  Blynk.virtualWrite(V6, message);
}

int readMoisture(){
  digitalWrite(moistPin, HIGH); // Turn the sensor ON
  delay(10);                    // Allow power to settle
  float val = analogRead(A0);   // Read the analog value form sensor
  digitalWrite(moistPin, LOW);  // Turn the sensor OFF
  return 100 - (val/1023.00)*100;
}

void updateWaterLevel(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distance = (duration*.0343)/2;
  waterLevel = (MAXHEIGHT - distance)/(MAXHEIGHT - 2)*100;
  Blynk.virtualWrite(V3, waterLevel);
  if (waterLevel <= 0){
    sendNotification("OUT OF WATER!!");
  }
  else
    sendNotification(" ");
}

void water(){
  int signal = digitalRead(relayPin);
  digitalWrite(relayPin, !signal);
  delay(waterDuration*1000);
  digitalWrite(relayPin, signal);
}

// Joystick handling
void smartCar(){
  int minRange = CENTER - 20;
  int maxRange = CENTER + 20;
  if (y > maxRange) {
    carForward();
    Serial.println("carForward");
  }
  else if (y < minRange) {
    carBackward();
    Serial.println("carBackward");
  }
  else if (x < minRange) {
    carLeft();
    Serial.println("carLeft");
  }
  else if (x > maxRange) {
    carRight();
    Serial.println("carRight");
  }
  else if (minRange <= x && x <= maxRange 
            && minRange <= y && y <= maxRange){
    carStop();
    Serial.println("carStop");
  }
}

// Motor movement functions
void carForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carStop() {
  int motors[] = {IN1, IN2, IN3, IN4};
  for(int i = 0; i < 4; i++)
    digitalWrite(motors[i], LOW);
}