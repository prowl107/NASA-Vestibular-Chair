#include <Wire.h>
#include <Arduino.h>

// Variables.
#define BUTTON_PIN 5
// #define pot  A1


const uint8_t smcDeviceNumber = 13;

// Required to allow motors to move.
// Must be called when controller restarts and after any error.
void exitSafeStart() {
  Wire.beginTransmission(smcDeviceNumber);
  Wire.write(0x83);  // Exit safe start
  Wire.endTransmission();
}

void setMotorSpeed(int16_t speed) {
  uint8_t cmd = 0x85;  // Motor forward
  if (speed < 0) {
    cmd = 0x86;  // Motor reverse
    speed = -speed;
  }
  Wire.beginTransmission(smcDeviceNumber);
  Wire.write(cmd);
  Wire.write(speed & 0x1F);
  Wire.write(speed >> 5 & 0x7F);
  Wire.endTransmission();
}

uint16_t readUpTime() {  // kinda un needed...
  Wire.beginTransmission(smcDeviceNumber);
  Wire.write(0xA1);  // Command: Get variable
  Wire.write(28);    // Variable ID: Up time (low)
  Wire.endTransmission();
  Wire.requestFrom(smcDeviceNumber, (uint8_t)2);
  uint16_t upTime = Wire.read();
  upTime |= Wire.read() << 8;
  return upTime;
}

void speedControlPot() {

  int pot = analogRead(A1);
  // Convert the analog reading (which goes from 0 - 1023) to a pot position 0- 1000 ohms)
  float potV = pot * (100 / 1023.0);
  //  Serial.println(potV);
  int motSpeed = (((potV)*32));



  setMotorSpeed(motSpeed);


  // byte buttonState = digitalRead(BUTTON_PIN);

  // while (digitalRead(BUTTON_PIN == LOW)) {

  //   setMotorSpeed(0);

  //   if (digitalRead(BUTTON_PIN == HIGH)) {
  //     break;
  //   }
  // }

    Serial.println(potV);

}

void eStop() {  // code for estop... will use later
}




void setup() {
  Serial.begin(9600);
  Wire.begin();
  exitSafeStart();

  pinMode(5, INPUT);  // set pin of e stop button as input
  digitalWrite(5, LOW);
}

void loop() {
  // Read the up time from the controller and send it to
  // the serial monitor.
  // uint16_t upTime = readUpTime();
  // Serial.print(F("Up time: "));
  // Serial.println(upTime);

  // setMotorSpeed(0);
  delay(10);
  speedControlPot();

  // setMotorSpeed(-3200);  // full-speed reverse
  // delay(1000);
}