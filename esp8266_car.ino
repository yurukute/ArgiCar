#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//FIRMWARE CONFIGURATION
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLwxz7Etgp"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "s3_jVWTCNlIVRDlTlMGAvxWOXxsJ84PT"

// Define motor pins
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4

// Joystick's default values
int x = 50;
int y = 50;

void setup() {
  Serial.begin(9600);
  //Set the motor pins as output pins
  int motors[] = {IN1, IN2, IN3, IN4};
  for(int i = 0; i < 4; i++)
    pinMode(motors[i], OUTPUT);
  // Initialize the Blynk library
  char ssid[] = ""; //Enter your WIFI name
  char pass[] = ""; //Enter your WIFI password
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
}

void loop() {
  Blynk.run(); // Run the blynk function
  if (y > 70) {
    carForward();
    Serial.println("carForward");

  } 
  else if (y < 30) {
    carBackward();
    Serial.println("carBackward");
  } 
  else if (x < 30) {
    carLeft();
    Serial.println("carLeft");
  } 
  else if (x > 70) {
    carRight();
    Serial.println("carRight");
  } 
  else if (x < 70 && x > 30 && y < 70 && y > 30) {
    carStop();
    Serial.println("carStop");
  }
}

//Get Blynk values
BLYNK_WRITE(V0) {
  x = param[0].asInt();
}

BLYNK_WRITE(V1) {
  y = param[0].asInt();
}

//Motor movement functions
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