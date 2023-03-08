/**********************************************************************************
* File: chair_pins.h
*
* Purpose: This file contains the pin mapping for the various switches
*           and peripherals of the system including arming switches, buttons,
            and potentiometers
**********************************************************************************/
#ifndef _CHAIR_PINS_H
#define _CHAIR_PINS_H

/* Libraries and dependcies */

/* Buttons and Switches */
#define BUTTON_PIN 5
#define CONFIRMATION_BTN 6
#define REJECT_BTN 7
#define ARMING_SWITCH_1 8
#define ESTOP_BTN   5
#define DIRECTION_BTN 2

/* System LEDs*/
#define DISARMED_LED 3
#define ARMED_LED 4
/* Potentiometer pins */
#define RPM_POTENTIOMETER A1
#define TIME_POTENTIOMETER A2
#endif  