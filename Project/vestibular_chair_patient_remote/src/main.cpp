#include <Arduino.h>
#include "RH_ASK.h"
#include <SPI.h>

/* General Macros */
#define RUNNING           1

/* I/O & Button Pins */
#define LEFT_BTN          4
#define RIGHT_BTN         2
#define EMERGENCY_STOP    3

/* Message packets */

/* Peripherals */
RH_ASK transmitter(2000, 11, 12, 10, false);


void setup() {
/* Message packets */
char msg[6];
uint8_t msg_len = 6;
const char *no_btn_signal= "None_";
const char *left_btn_signal  =  "Left_";
const char *right_btn_signal =  "Right_";
const char *multi_btn_signal =  "Both_";
const char *estop_btn_signal =  "STOP_";
const char *start_signal = "START";

Serial.begin(115200);
Serial.println("Program start");

  /* Initialize I/O Pins */
  pinMode(LEFT_BTN, INPUT_PULLUP);
  pinMode(RIGHT_BTN, INPUT_PULLUP);
  pinMode(EMERGENCY_STOP, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);	
  uint8_t led_state = 0;

  /* Initialize any communication interfaces */
  // *msg = no_btn_signal;
  msg_len = sizeof(msg);
  
  if (transmitter.init())
  {
       Serial.println("init sender");
  }

  for(uint8_t i = 0; i < 25; i++)
  {
    transmitter.send((uint8_t *)start_signal, strlen(start_signal));
    delay(100);
  }

  for (int i = 0; i < 6; i++)
  {
    msg[i] = 0;
  }


while(true)
  {
    /* Emergency stop detected */
    if(digitalRead(EMERGENCY_STOP) == LOW)
    {
      // Serial.println("Emergency button pressed");
      // strcpy(msg, estop_btn_signal);
      // strcpy(msg, estop_btn_signal);
      transmitter.send((uint8_t *)estop_btn_signal, strlen(estop_btn_signal));
      delay(2500);
        for(uint8_t i = 0; i < 10; i++)
        {
        transmitter.send((uint8_t *)start_signal, strlen(start_signal));
        delay(200);
        }
            transmitter.send((uint8_t *)no_btn_signal, strlen(no_btn_signal));
      continue;
    }

    /* Right button pressed */
    else if(digitalRead(RIGHT_BTN) == LOW && digitalRead(LEFT_BTN) == HIGH)
    {
      // Serial.println("Right button pressed");
      // strcpy(msg, right_btn_signal);
      // msg = right_btn_signal;
      transmitter.send((uint8_t *)right_btn_signal, strlen(right_btn_signal));
    }

    /* Left button pressed */
    else if(digitalRead(RIGHT_BTN) == HIGH && digitalRead(LEFT_BTN) == LOW)
    {
      // Serial.println("Left button pressed");
      transmitter.send((uint8_t *)left_btn_signal, strlen(left_btn_signal));
      // strcpy(msg, left_btn_signal);
      // msg = left_btn_signal;
    }

    /* Multiple buttons pressed*/
    else if(digitalRead(RIGHT_BTN) == LOW && digitalRead(LEFT_BTN) == LOW)
    {
      // Serial.println("Multiple buttons pressed");
      transmitter.send((uint8_t *)multi_btn_signal, strlen(multi_btn_signal));
      // strcpy(msg, multi_btn_signal);
      // msg = multi_btn_signal;
    }
    /* If no button is pressed */
    else {
      // Serial.println("No button pressed");
      transmitter.send((uint8_t *)no_btn_signal, strlen(no_btn_signal));
      // strcpy(msg, no_btn_signal);
      // msg = no_btn_signal;
    }

    // transmitter.send((uint8_t *) msg, msg_len);    
    // transmitter.waitPacketSent();
    digitalWrite(LED_BUILTIN,led_state);
    if(led_state <= 0)
    {
      led_state = 1;
    }else{
      led_state = 0;
    }

  }
}

void loop() {} /* Nothing here */