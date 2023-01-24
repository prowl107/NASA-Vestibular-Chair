/**********************************************************************************
* File: chair_motor_control_lib.h
*
* Purpose: Library interface for the Pololu Simple Motor Controller
**********************************************************************************/

#ifndef _CHAIR_MOTOR_CONTROL_LIB_
#define _CHAIR_MOTOR_CONTROL_LIB_

/* Libraries and dependencies */
#include <Wire.h>
#include <Arduino.h>

/* Macros */
#define BUTTON_PIN 5
#define SMCDEVICENUMBER 13

/* Types */

class SMC
{
    public:
    SMC(uint8_t potentiometer_pin = A1);
    /* Public Function Prototypes */
    void exitSafeStart();
    void setMotorSpeed(int16_t speed);
    uint16_t readUpTime();
    int16_t controlPot();
    long potScaler(uint8_t pin);
    void eStop();

    private:
    int16_t target_speed; 
    uint16_t upTime;
    uint8_t pin;

};

#endif /* CHAIR_MOTOR_CONTROL_LIB_ */