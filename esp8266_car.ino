#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//FIRMWARE CONFIGURATION
#define BLYNK_TEMPLATE_ID "TMPLwxz7Etgp"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "s3_jVWTCNlIVRDlTlMGAvxWOXxsJ84PT"

// Variables for the Blynk widget values
int x = 50;
int y = 50;

// Get the joystick values
BLYNK_WRITE(V0) {
  x = param[0].asInt();
}
// Get the joystick values
BLYNK_WRITE(V1) {
  y = param[0].asInt();
}

// Define the motor pins
const int motors[5] = {0, D1, D2, D3, D4};

void setup() {
  Serial.begin(115200);
  //Set the motor pins as output pins
  for(int i = 1; i <= 4; i++)
    pinMode(motors[i], OUTPUT);
  // Initialize the Blynk library
  char ssid[] = ""; //Enter your WIFI name
  char pass[] = ""; //Enter your WIFI passowrd
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
}

// Check these values using the IF condition
void smartCar() {
  if (y > 70) {
    carForward();
    Serial.println("carForward");
  } else if (y < 30) {
    carBackward();
    Serial.println("carBackward");
  } else if (x < 30) {
    carLeft();
    Serial.println("carLeft");
  } else if (x > 70) {
    carRight();
    Serial.println("carRight");
  } else if (x < 70 && x > 30 && y < 70 && y > 30) {
    carStop();
    Serial.println("carStop");
  }
}

void loop() {
  Blynk.run();// Run the blynk function
  smartCar();// Call the main function
}

/**************Motor movement functions*****************/
void carForward() {
  for(int i = 1; i <= 4; i++){
    digitalWrite(motors[i], (i % 2) ? HIGH : LOW);
  }
}
void carBackward() {
  for(int i = 1; i <= 4; i++){
    digitalWrite(motors[i], (i % 2) ? LOW : HIGH);
  }
}

void carLeft() {
  for(int i = 1; i <= 4; i++){
    digitalWrite(motors[i], (i > 1 && i < 4) ? LOW : HIGH);
  }
}

void carRight() {
  for(int i = 1; i <= 4; i++){
    digitalWrite(motors[i], (i > 1 && i < 4) ? HIGH : LOW);
  }
}

void carStop() {
  for(int i = 1; i <= 4; i++)
    digitalWrite(motors[0], LOW);
}