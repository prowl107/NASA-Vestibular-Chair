/**********************************************************************************
* File: chair_lib_gbl.h
*
* Global library for the <Insrt project name> chair consisting of
*   - chair pins
**********************************************************************************/
#ifndef GBL_CHAIR_LIB
#define GBL_CHAIR_LIB

/* Individual libraries */
#include "chair_pins.h"
#include "chair_fsm_lib.h"
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "chair_motor_control_lib.h"
#include <LiquidCrystal_I2C.h> // includes the LiquidCrystal Library 

/* Dependencies */
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Arduino.h>

/* General macros */
#define CLOCKWISE 0
#define COUNTERCLOCKWISE 1

/* Types */

/* Function Prototypes */
void sys_init();
void sys_display_init(LiquidCrystal_I2C *lcd);
void sys_io_init();
void emergency_stop();

# endif