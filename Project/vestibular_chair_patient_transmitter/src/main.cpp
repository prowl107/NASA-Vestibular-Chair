#include <Arduino.h>
#include "RH_ASK.h"
#include <SPI.h>

/* General Macros */
#define RUNNING           1

/* I/O & Button Pins */
#define LEFT_BTN          2
#define RIGHT_BTN         3
#define EMERGENCY_STOP    4

/* Message packets */
#define LEFT_BTN_SIGNAL   "1"
#define RIGHT_BTN_SIGNAL  "2"
#define MULTI_BTN_SIGNAL  "3"
#define ESTOP_BTN_SIGNAL  "4"

/* Peripherals */
RH_ASK transmitter;
char msg[12];
uint8_t msg_len;


void setup() {
  /* Initialize I/O Pins */
  pinMode(LEFT_BTN, INPUT_PULLUP);
  pinMode(RIGHT_BTN, INPUT_PULLUP);
  pinMode(EMERGENCY_STOP, INPUT_PULLUP);

  /* Initialize any communication interfaces */
  *msg = 0;
  msg_len = sizeof(msg);
  transmitter.init();

while(true)
  {
    /* Emergency stop detected */
    if(digitalRead(EMERGENCY_STOP) == LOW)
    {
      *msg = "Hello Worls";
      transmitter.send((uint8_t *)msg, msg_len);
      delay(2000);
      continue;
    }

    /* Right button pressed */
    if(digitalRead(RIGHT_BTN) == LOW && digitalRead(LEFT_BTN) == HIGH)
    {
      while(digitalRead(RIGHT_BTN) == LOW)
      {
      *msg = RIGHT_BTN_SIGNAL;
      transmitter.send((uint8_t *)msg, msg_len);
      }
    }

    /* Left button pressed */
    if(digitalRead(RIGHT_BTN) == HIGH && digitalRead(LEFT_BTN) == LOW)
    {
      while(digitalRead(LEFT_BTN) == LOW)
      {
      *msg = LEFT_BTN_SIGNAL;
      transmitter.send((uint8_t *)msg, msg_len);
      }
    }

    /* Multiple buttons pressed*/
    if(digitalRead(RIGHT_BTN) == LOW && digitalRead(LEFT_BTN) == LOW)
    {
      while(digitalRead(RIGHT_BTN) == LOW && digitalRead(LEFT_BTN) == LOW)
      {
      *msg = MULTI_BTN_SIGNAL;
      transmitter.send((uint8_t *) msg, msg_len);
      }
    }    
  }
}

void loop() {} /* Nothing here */