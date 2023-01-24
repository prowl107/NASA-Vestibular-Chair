/**********************************************************************************
* File: main.cpp
*
* Purpose: Application entry point
**********************************************************************************/
#include "chair_lib_gbl.h"
#include "chair_pins.h"
#include <LiquidCrystal_I2C.h> // includes the LiquidCrystal Library 
#include <RadioHead.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>

/* Peripherals */
LiquidCrystal_I2C lcd(0x27, 20, 4);
FSM fsm;
SMC smc; /* Simple motor controller */
RH_ASK receiver; /* Wireless transmitter/receiver */

void setup() {
Serial.begin(115200);
/*************************************************************************
* Local Variables
*************************************************************************/ 

/* Test sequence duration */
long test_start_time;                                     
long test_target_time;
long test_time_elapsed;
int32_t time_left;
uint8_t counter;

/* Finite State Machine (FSM) parameters */
SYS_STATES_T state_index;

/* Motor controller target parameters */
int16_t current_speed; 
int16_t speed_target;
float speed_target_percent = 0.0;
uint32_t rpm_encoder_val = 0;
uint32_t rpm_encoder_prev_state = analogRead(A0);
uint32_t rpm_encoder_new_state;
int32_t speed_target_scaled;

/*************************************************************************
* Direction control
* 0 - Clockwise (CW) ~ default
* 1 - Counterclockwise (CCW)
*************************************************************************/ 
uint8_t direction = CLOCKWISE;
/* Wireless Communication parameters */
/*************************************************************************
* Expected Messages: 
* No button-press: 0, no message should be recieved in this case
* Left button press: 1
* Right button press: 2
* Both button presses: 3
*************************************************************************/
uint8_t *msg;
uint8_t msg_len = sizeof(msg);

// /*************************************************************************
// * Initialize Local Variables
// *************************************************************************/ 
smc = SMC(RPM_POTENTIOMETER);
counter = 0;
fsm = FSM(SYS_INIT);
test_start_time = 0;  
test_target_time = 0;
test_time_elapsed = 0;
state_index = SYS_INIT;
current_speed = 0;
speed_target = 0;

/*************************************************************************
* Start system Operation:
*
* States:
* 1 - SYS_INIT
* 2 - PROCTOR_OP_RPM 
* 3 - PROCTOR_OP_TIME
* 4 - SYS_ARM_HOLD
* 5 - ARMED_OPERATION
* 6 - ARMED_TARGET_REACHED
**************************************************************************/
while(1){
switch (fsm.getCurrentState())
{
case SYS_INIT:
// pinMode(DISARMED_LED, OUTPUT);
// pinMode(ARMED_LED, OUTPUT);
// digitalWrite(DISARMED_LED, HIGH);
// digitalWrite(ARMED_LED, HIGH);
// delay(1000);
// digitalWrite(DISARMED_LED, LOW);
// digitalWrite(ARMED_LED, LOW);
  Serial.println("NASA Vestibular Chair -- Loading interface...");
  Wire.begin(); 
  Wire.setClock(400000);

  /* Initialize I/O Pins */
  sys_io_init();

  /* Initialize emergency stop button */
  // attachInterrupt(digitalPinToInterrupt(ESTOP_BTN), emergency_stop, CHANGE);

  /* Initialize motor controller & interface */
  smc.exitSafeStart();

  /* Initialize displays */
  sys_display_init(&lcd);

  /* Initialize web interface */

  /* Initialize connection to wireless peripheral */
  receiver.init();//

  /* System disarmed status */
  // digitalWrite(DISARMED_LED, HIGH);
  // digitalWrite(ARMED_LED, HIGH);

  /* Transition to PROCTOR_OP_RPM */
  state_index = PROCTOR_OP_RPM;
  break;

case PROCTOR_OP_RPM:

  /***********************************************************************
  * Retrieve target speed from analog input
  ***********************************************************************/ 
  speed_target = smc.controlPot(); /* NOTE: pass rpm_encoder target to smc.setMotorSpeed() during operation*/
  speed_target_percent = (speed_target/3200.0)*100; 
  speed_target_scaled = round((0.015833333333333335 * speed_target) - 8.916666666666671);
      Serial.print("speed_target_scaled: ");
    Serial.println(speed_target_scaled);
  if(speed_target_scaled < 0)
  {
    speed_target_scaled = 0;
  }

  lcd.setCursor(strlen("Speed(RPM): "), 0);
  lcd.print(speed_target_scaled,10);

  /* Clear trailing digit from display if less than 1000*/
  if(speed_target_scaled<1000)
  {
    lcd.setCursor(15, 0);
    lcd.print(' ');
  }

  if(speed_target_scaled<100)
  {
    lcd.setCursor(14, 0);
    lcd.print(' ');
  }

  if (speed_target_scaled<10)
  {
    lcd.setCursor(13,0);
    lcd.print(' ');
  }

  if (digitalRead(CONFIRMATION_BTN) == LOW)
  {
    /* Wait until button depress */
    while(digitalRead(CONFIRMATION_BTN) == LOW);
    state_index = PROCTOR_OP_TIME;
  }

  break;

// case PROCTOR_OP_DIRECTION:

// if(smc.controlPot() < 1600)
// {
//   direction = COUNTERCLOCKWISE;
//   lcd.setCursor(strlen("Spin: "),1);
//   lcd.print("CClockwise");
// }else if (smc.controlPot() >= 1601) {
//   direction = CLOCKWISE;
//   lcd.setCursor(strlen("Spin: "),1);
//   lcd.print("Clockwise  ");
// }

// /************************************************************************
// * @todo Update display with new display
// ************************************************************************/
// lcd.setCursor(0,1);
// lcd.print("Spin: ");
//  if (digitalRead(CONFIRMATION_BTN) == LOW && digitalRead(REJECT_BTN) == HIGH)
//   {
//     /* Wait until button depress */
//     while(digitalRead(CONFIRMATION_BTN) == LOW);
//     state_index = PROCTOR_OP_TIME;
//   }else if(digitalRead(CONFIRMATION_BTN) == HIGH && digitalRead(REJECT_BTN) == LOW) /* Return to previous state */
//   {
//     /* Wait until button depress */
//     while(digitalRead(REJECT_BTN) == LOW);
//     state_index = PROCTOR_OP_RPM;
//   }
// break;

case PROCTOR_OP_TIME: 
  /***********************************************************************
  * Retrieve target duration from analog input
  ***********************************************************************/ 
  test_target_time = map(smc.controlPot(), 0, 3200, 0, 120); /* NOTE: pass rpm_encoder target to smc.setMotorSpeed() during operation*/

  lcd.setCursor(strlen("Duration: "), 1);
  lcd.print(test_target_time,10);

  /* Clear trailing digit from display if less than 1000*/
  if(test_target_time<100)
  {
    lcd.setCursor(12, 1);
    lcd.print(' ');
    if(test_target_time < 10)
    {
    lcd.setCursor(11, 1);
    lcd.print(' ');
    }
  }

  if (digitalRead(CONFIRMATION_BTN) == LOW && digitalRead(REJECT_BTN) == HIGH)
  {
    /* Wait until button depress */
    while(digitalRead(CONFIRMATION_BTN) == LOW);
    state_index = SYS_ARM_HOLD;
  }else if(digitalRead(CONFIRMATION_BTN) == HIGH && digitalRead(REJECT_BTN) == LOW) /* Return to previous state */
  {
    /* Wait until button depress */
    while(digitalRead(REJECT_BTN) == LOW);
    state_index = PROCTOR_OP_RPM;
  }

  Serial.print("\tTime: ");
  Serial.print("speed_target_scaled: ");
  Serial.println(test_target_time);
  break;

case SYS_ARM_HOLD:
  lcd.setCursor(strlen("Status: "), 2);
  /* Return to previous state */
  if (digitalRead(REJECT_BTN) == LOW)
  {
    lcd.print("        ");
    lcd.setCursor(strlen("Status: "), 2);
    lcd.print("Disarmed");
    state_index = PROCTOR_OP_TIME;    
    /* Wait until button depress */
    while(digitalRead(REJECT_BTN) == LOW);
    break;
  }

  /* Wait until button/switch confirmation before allowing operation */
  if (digitalRead(ARMING_SWITCH_1) == HIGH)
  {
    lcd.print("        ");
    lcd.setCursor(strlen("Status: "), 2);
    lcd.print("Disarmed");
    state_index = SYS_ARM_HOLD;
    digitalWrite(DISARMED_LED, HIGH);
    digitalWrite(ARMED_LED, LOW);
    break;

  }else if(digitalRead(ARMING_SWITCH_1) == LOW){
    lcd.print("        ");
    lcd.setCursor(strlen("Status: "), 2);
    lcd.print("Armed");
    digitalWrite(DISARMED_LED, LOW);
    digitalWrite(ARMED_LED, HIGH);
  
   if(digitalRead(CONFIRMATION_BTN) == LOW) {
    lcd.print("       ");
    lcd.setCursor(strlen("Status: "), 2);
    /* Wait until button depress */
    while(digitalRead(CONFIRMATION_BTN) == LOW);
    state_index = ARMED_OPERATION;
    lcd.print("ARMED_OP");
  }
  }

  break;

case ARMED_OPERATION:
  test_start_time = millis();
  test_time_elapsed = millis();
  test_target_time = test_target_time*1000; /* Convertfrom S to MS */

  lcd.setCursor(0,3);
  lcd.print("Time left: ");
  /*********************************************************************
  * Gradually increase RPM until target is reached
  **********************************************************************/
 while(state_index == ARMED_OPERATION)
 {
  /*********************************************************************
  * Emergency Stop Detected --> Stop Test Immediately
  **********************************************************************/
  if(digitalRead(ESTOP_BTN) == LOW)
  {
    emergency_stop();
    state_index = SYS_INIT;
    break;
  }

  /**********************************************************************
  * Capture patient data from wireless device
  * and send to PC
  * 
  * @TODO: Send data to PC! (pyserial)
  **********************************************************************/
  //receiver.recv(msg, &msg_len); 

  //  if(test_target_time+1000 > (test_time_elapsed - test_start_time))
  // {
  //       if(current_speed < speed_target)
  //       {
  //         smc.setMotorSpeed(current_speed);
  //         current_speed+=2;
  //         // state_index = ARMED_TARGET_REACHED;
  //       }else{
  //         smc.setMotorSpeed(current_speed);
  //         // state_index = ARMED_TARGET_REACHED;
  //       }

  //     // case COUNTERCLOCKWISE:
  //     //   if(-current_speed > -speed_target)
  //     //   {
  //     //     smc.setMotorSpeed(-current_speed);
  //     //     current_speed+=2;
  //     //     // state_index = ARMED_TARGET_REACHED;
  //     //   }else{
  //     //     smc.setMotorSpeed(-current_speed);
  //     //     // state_index = ARMED_TARGET_REACHED;
  //     //   }
  //   }

  if(test_target_time+1000 > (test_time_elapsed - test_start_time))
  {
    if(current_speed < speed_target)
    {
    smc.setMotorSpeed(current_speed);
    current_speed+=3;
    // state_index = ARMED_TARGET_REACHED;
    }else{
      smc.setMotorSpeed(current_speed);
      // state_index = ARMED_TARGET_REACHED;
    }
    test_time_elapsed = millis();
  /* Print Time left on last line of the LCD: */
  lcd.setCursor(strlen("Time left: 0")-1, 3);
  time_left = ((test_target_time - (test_time_elapsed - test_start_time))/1000);
  lcd.print(time_left);  
  }

  if(time_left<100)
  {
    lcd.setCursor(13, 3);
    lcd.print(' ');
    if(time_left<10){
      lcd.print(' ');
     lcd.setCursor(12, 3);
     lcd.print(' ');
    }
  }

  /*********************************************************************
  * If set duration is reached move to ARNED_TARGET_REACHED state
  **********************************************************************/
  if(time_left <= 0)
  {
    state_index = ARMED_TARGET_REACHED;
  }
  
  test_time_elapsed = millis();
 }
  break; 

case ARMED_TARGET_REACHED:
  /*********************************************************************
  * Gradually decrease RPM until target is reached
  **********************************************************************/
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Test Complete");
 lcd.setCursor(0,1);
 lcd.print("Slowing down");
    while(current_speed > 0)
    {
    current_speed-=50;
    state_index = ARMED_TARGET_REACHED;
    /**********************************************************************
    * Capture patient data from wireless device
    * and send to PC
    * 
    * @TODO: Send data to PC! (pyserial)
    **********************************************************************/
    receiver.recv(msg, &msg_len); 
    smc.setMotorSpeed(current_speed);
    delay(200);
    }


  /**********************************************************************
  * @todo Finish logging data.
  **********************************************************************/
lcd.setCursor(0,2);
lcd.print("Saving data to file");
/* Insert end of file signal */
delay(5000);
lcd.setCursor(0,3);
lcd.print("Done");
delay(3000);

  /**********************************************************************
  * Reset for next sequence
  **********************************************************************/
 sys_display_init(&lcd);
 *msg = 0;
 state_index = SYS_INIT;

  break;

case INVALID:
  /* Do Nothing */
  break;

default:
  break;
}

/*************************************************************************
* Update FSM
*************************************************************************/
Serial.print("Current State index: ");
Serial.print(fsm.getCurrentState());
Serial.print("\t");
Serial.print("Next state index: ");
Serial.println(state_index);
fsm.setCurrentState(state_index);
delay(25); 
}

for(;;);
}

void loop(){} /* Do Nothing */

void sys_display_init(LiquidCrystal_I2C *lcd)
{
  lcd->init();
  lcd->backlight();
  /* Print 'Speed!' on the first line of the LCD: */
  lcd->setCursor(0, 0); // Set the cursor on the third column and first row.
  lcd->print("Speed(RPM): 0");

    /* Print spin direction on the second line of the LCD: */
  // lcd->setCursor(0,1);
  // lcd->print("Spin: "); // Print the string "Hello World!"

  /* Print Duration on the second line of the LCD: */
  lcd->setCursor(0,1);
  lcd->print("Duration: 0"); // Print the string "Hello World!"

  /* Print Status on the third line of the LCD: */
  lcd->setCursor(0,2);
  lcd->print("Status: DISARMED");

  /* Print Time left on last line of the LCD: */
  lcd->setCursor(0,3);
  lcd->print("Time left: 0");
}

void sys_io_init()
{
  pinMode(CONFIRMATION_BTN, INPUT_PULLUP);
  pinMode(REJECT_BTN, INPUT_PULLUP);
  pinMode(ARMING_SWITCH_1, INPUT_PULLUP);
  pinMode(ESTOP_BTN, INPUT_PULLUP);
}

void emergency_stop()
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("EMERGENCY STOP");
    smc.eStop();
  delay(2000);
  fsm.setCurrentState(SYS_INIT);
}

#if 0
/*************************************************************************
* Begin State Machine:
*
* States:
* 1 - SYS_INIT
* 2 - PROCTOR_OP_RPM 
* 3 - PROCTOR_OP_TIME
* 4 - SYS_ARM_HOLD
* 5 - ARMED_OPERATION
* 6 - ARMED_TARGET_REACHED
**************************************************************************/
void begin_FSM()
{

/*************************************************************************
* Local Variables
**************************************************************************/
SYS_STATES_T state = SYS_INIT;                            /* FSM State managements */
uint32_t rpm; 
uint32_t rpm_encoder_target;
uint32_t rpm_encoder_val = 0;
uint32_t rpm_encoder_prev_state = analogRead(A0);
uint32_t rpm_encoder_new_state;

uint32_t time_encoder_val;
uint32_t time_encoder_prev_state = analogRead(A1);
uint32_t time_encoder_new_state;

long test_start_time;                                     /* Test sequence duration */
long test_target_time;
long test_time_elapsed;

while(1)
{
switch (state)
{
  case (SYS_INIT):
  Serial.println("NASA Vestibular Chair -- Loading interface...");

  /* Initialize all peripherals */
  display_init();

  //Motor controller initialization here:

  /* Transition to PROCTOR_OP_TIME state */
  rpm_encoder_new_state = analogRead(A0);
  state = PROCTOR_OP_RPM;
  break;

  case (PROCTOR_OP_RPM):
  Serial.println("PROCTOR_OP_RPM");
  while(1)
  {
    /* Transition to next state if unique button press*/
    if(digitalRead(2) == HIGH)
    {
      state = PROCTOR_OP_TIME;
    }

    rpm_encoder_new_state = analogRead(A0);
    if(rpm_encoder_new_state+2 < rpm_encoder_prev_state)
    {
      Serial.print("Decrase\t");
      if(rpm_encoder_target > 0)
      {
        rpm_encoder_target-=1;
      }
      rpm_encoder_prev_state = rpm_encoder_new_state;
    }else if(rpm_encoder_new_state-2 > rpm_encoder_prev_state)
    {
      Serial.print("Increase\t");
      if(rpm_encoder_target < 60)
      {
        rpm_encoder_target+=1;
      }
      rpm_encoder_prev_state = rpm_encoder_new_state;
    }else{
      Serial.print("No change\t");
    }
    rpm_disp.printNumber(rpm_encoder_target);
    rpm_disp.writeDisplay();
    Serial.println(analogRead(A0));
    delay(500);
  }

  break;

  case (PROCTOR_OP_TIME):
  
    if(digitalRead(3) == HIGH)
    {
      state = SYS_ARM_HOLD;
    }

    time_encoder_new_state = analogRead(A0);
    if(time_encoder_new_state+3 < time_encoder_prev_state)
    {
      Serial.print("Decrase\t");
      if(time_encoder_val > 0)
      {
        time_encoder_val-=1;
      }
      time_encoder_prev_state = time_encoder_new_state;
    }else if(time_encoder_new_state-3 > time_encoder_prev_state)
    {
      Serial.print("Increase\t");
      if(time_encoder_val < 60)
      {
        time_encoder_val+=1;
      }
      time_encoder_prev_state = time_encoder_new_state;
    }else{
      Serial.print("No change\t");
    }
    time_disp.printNumber(time_encoder_val);
    time_disp.writeDisplay();
    Serial.println(analogRead(A0));

    /*********************************************************************
    * @todo Set the test duration time witht the correct units 
    **********************************************************************/
    test_target_time = 0;
    delay(500);

  break;

  case(SYS_ARM_HOLD):
  while(!digitalRead(4))
  {
    state = SYS_ARM_HOLD;
    test_start_time = millis();
  }
  state = ARMED_OPERATION;
  
  break;

  case(ARMED_OPERATION):
  while(!(test_time_elapsed >= test_target_time) && !(rpm_encoder_val >= rpm_encoder_target))
  {
    /*********************************************************************
    * @todo Incorporate target angle setting either in DPS or RPM
    **********************************************************************/
   if(rpm_encoder_target < rpm_encoder_target)
   {
    rpm_encoder_target+=1;
   }
  }

  if(rpm_encoder_val >= rpm_encoder_target)
  {
    state = ARMED_TARGET_REACHED;
  }

  /*********************************************************************
  * @todo Implement set RPM/speed commands
  **********************************************************************/
  //motor_control.setSpeed(x);

  break;

  case(ARMED_TARGET_REACHED):
  /*********************************************************************
  * @todo Decrease chair speed by 1~1.5° gradually until RPM is 0
  **********************************************************************/
 while (rpm_encoder_val > 0)
 { 
  rpm_encoder_val -= 1;
  /*********************************************************************
  * @todo Implement set RPM/speed commands
  **********************************************************************/
  //motor_control.setSpeed(x); 
 }

  break;

  default:
  /* Hold chair at 0 RPM */
    break;
}
}
}
#endif

/*************************************************************************
* Function: sys_init
*
* Purpose: Initializes system peripherals and objects 
*************************************************************************/
// void sys_init()
// {

// }
