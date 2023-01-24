/**********************************************************************************
* File: chair_motor_control_lib.h
*
* Purpose: Library interface for the Pololu Simple Motor Controller
**********************************************************************************/
#include "chair_motor_control_lib.h"

SMC::SMC(uint8_t potentiometer_pin = A1){
    pin = A1;
}

/***********************************************************************************
* Function: exitSafeStart
*
* Required to allow motors to move.
* Must be called when controller restarts and after any error.
***********************************************************************************/
void SMC::exitSafeStart() {
  Wire.beginTransmission(SMCDEVICENUMBER);
  Wire.write(0x83);  // Exit safe start
  Wire.endTransmission();
}

/***********************************************************************************
* Function: setMotorSpeed
* 
***********************************************************************************/
void SMC::setMotorSpeed(int16_t speed) {
  uint8_t cmd = 0x85;  // Motor forward
  if (speed < 0) {
    cmd = 0x86;  // Motor reverse
    speed = -speed;
  }
  Wire.beginTransmission(SMCDEVICENUMBER);
  Wire.write(cmd);
  Wire.write(speed & 0x1F);
  Wire.write(speed >> 5 & 0x7F);
  Wire.endTransmission();
  target_speed = speed;
}

/***********************************************************************************
* Function: readUpTime
* 
***********************************************************************************/
uint16_t SMC::readUpTime() {  // kinda un needed...
  Wire.beginTransmission(SMCDEVICENUMBER);
  Wire.write(0xA1);  // Command: Get variable
  Wire.write(28);    // Variable ID: Up time (low)
  Wire.endTransmission();
  Wire.requestFrom(SMCDEVICENUMBER, (uint8_t)2);
  upTime = Wire.read();
  upTime |= Wire.read() << 8;
  return upTime;
}

/***********************************************************************************
* Function: speedControlPot
* 
***********************************************************************************/
int16_t SMC::controlPot() {

  float potV1 = potScaler(pin);
//   float potV2 = potScaler(TIME_POTENTIOMETER);

  int motSpeed = (((potV1*32)));
  int timeVal;
  
  Serial.print("Speed: ");
  Serial.print(motSpeed);
  Serial.print("\tTime: ");

  return motSpeed;


#if 0
  if (digitalRead(BUTTON_PIN) == LOW) {
    //does nothing until pressed
  } else {
    //sets values after the button is released
    while (digitalRead(BUTTON_PIN) == HIGH) {
      if (digitalRead(BUTTON_PIN) == LOW) {

        motSpeed = potV1;
        // timeVal = potV2;

        Serial.print("Speed: ");
        Serial.print(motSpeed);
        Serial.print("\tTime: ");
        Serial.print(timeVal);
        Serial.print("\n");
        break;
      }

      //motSpeed = 0;
      setMotorSpeed(motSpeed);
    }
    }
#endif
}

/***********************************************************************************
* Function: eStop
***********************************************************************************/
void SMC::eStop() 
{  
  setMotorSpeed(0);
}

long SMC::potScaler(uint8_t pin) {
  // Convert the analog reading (which goes from 0 - 1023) to a pot position 0- 1000 ohms)
  return analogRead(pin) * (100/1023.0);
}