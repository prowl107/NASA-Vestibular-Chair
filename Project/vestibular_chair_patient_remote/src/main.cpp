#include <Arduino.h>
#include <SoftwareSerial.h>
#include <bluefruit52.h>
#include <SPI.h>

/* General Macros */
#define RUNNING           1

/* I/O & Button Pins */
#define LEFT_BTN          12
#define RIGHT_BTN         13
#define EMERGENCY_STOP    11

/* Message packets */

// BLE Service
BLEDfu  bledfu;  // OTA DFU service
BLEDis  bledis;  // device information
BLEUart bleuart; // uart over ble
BLEBas  blebas;  // battery



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

 // Initialize hardware:
  Serial.begin(115200); // Serial is the USB serial port

  /* Initialize I/O Pins */
  pinMode(LEFT_BTN, INPUT_PULLUP);
  pinMode(RIGHT_BTN, INPUT_PULLUP);
  pinMode(EMERGENCY_STOP, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);	
  uint8_t led_state = 0;


  // Initialize Bluetooth:
  Bluefruit.begin();
  // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  Bluefruit.setName("PatientTx");
  bleuart.begin();

  // Start advertising device and bleuart services
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(bleuart);
  Bluefruit.ScanResponse.addName();

  Bluefruit.Advertising.restartOnDisconnect(true);
  // Set advertising interval (in unit of 0.625ms):
  Bluefruit.Advertising.setInterval(32, 244);
  // number of seconds in fast mode:
  Bluefruit.Advertising.setFastTimeout(30);
  Bluefruit.Advertising.start(0);  


  /* Initialize any communication interfaces */
  // *msg = no_btn_signal;
  msg_len = sizeof(msg);
  


  while(true) {
      // Serial.print(millis());
      // Serial.print("\t");
      /* Emergency stop detected */
      #if 0
      if(digitalRead(EMERGENCY_STOP) == LOW)
      {
        Serial.println("Emergency button pressed");
        // strcpy(msg, estop_btn_signal);
        // strcpy(msg, estop_btn_signal);
        bleuart.write((uint8_t *)estop_btn_signal, strlen(estop_btn_signal));
        delay(2500);
          for(uint8_t i = 0; i < 10; i++)
          {
          bleuart.write((uint8_t *)start_signal, strlen(start_signal));
          delay(200);
          }
              bleuart.write((uint8_t *)no_btn_signal, strlen(no_btn_signal));
        continue;
      }
      #endif

      /* Right button pressed */
      if(digitalRead(RIGHT_BTN) == LOW && digitalRead(LEFT_BTN) == HIGH)
      {
        // Serial.println("Right button pressed");
        // strcpy(msg, right_btn_signal);
        // msg = right_btn_signal;
        bleuart.write((uint8_t *)right_btn_signal, strlen(right_btn_signal));
      }

      /* Left button pressed */
      else if(digitalRead(RIGHT_BTN) == HIGH && digitalRead(LEFT_BTN) == LOW)
      {
        // Serial.println("Left button pressed");
        bleuart.write((uint8_t *)left_btn_signal, strlen(left_btn_signal));
        // strcpy(msg, left_btn_signal);
        // msg = left_btn_signal;
      }

      /* Multiple buttons pressed*/
      else if(digitalRead(RIGHT_BTN) == LOW && digitalRead(LEFT_BTN) == LOW)
      {
        // Serial.println("Multiple buttons pressed");
        bleuart.write((uint8_t *)multi_btn_signal, strlen(multi_btn_signal));
        // strcpy(msg, multi_btn_signal);
        // msg = multi_btn_signal;
      }
      /* If no button is pressed */
      else {
        // Serial.println("No button pressed");
        bleuart.write((uint8_t *)no_btn_signal, strlen(no_btn_signal));
        // strcpy(msg, no_btn_signal);
        // msg = no_btn_signal;
      }

      // bleuart.write((uint8_t *) msg, msg_len);    
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

// void loop() {
//   char data[32];

//   bool b1 = digitalRead(BUTTON_Right);
//   bool b2 = digitalRead(BUTTON_Left);
//   uint8_t state = b2 - b1 + 1;

//   switch (state) {
//     case  0: strcpy(data,"Left");   break;
//     case  1: strcpy(data,"None");   break;
//     case  2: strcpy(data,"Right");  break;
//   }
  
//   // uint32_t time = millis();
//   // itoa(time,data,10); 
//   bleuart.write(data);
//   delay(10);

// }
