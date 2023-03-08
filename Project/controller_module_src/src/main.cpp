/**********************************************************************************
* File: main.cpp
*
* Purpose: Application entry point
**********************************************************************************/
#include "chair_lib_gbl.h"
#include "chair_pins.h"
#include <LiquidCrystal_I2C.h> // includes the LiquidCrystal Library 
#include <bluefruit.h>
// #include <bluefruit52.h>
// #include <RadioHead.h>
// #include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>

/* Peripherals */
LiquidCrystal_I2C lcd(0x27, 20, 4);
FSM fsm(SYS_INIT);
SMC smc(A1); /* Simple motor controller */
// RH_ASK receiver; /* Wireless transmitter/receiver */
uint8_t start_test_flag;


char rx[6];


BLEClientBas  clientBas;  // battery client
BLEClientDis  clientDis;  // device information client
BLEClientUart clientUart; // bleuart client

void connect_callback(uint16_t conn_handle);
void scan_callback(ble_gap_evt_adv_report_t* report);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);
void bleuart_rx_callback(BLEClientUart& uart_svc);

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
start_test_flag = 0;
long chair_stop_timestamp;
long end_of_test_timestamp;
uint8_t chair_stop_flag;

/* Finite State Machine (FSM) parameters */
SYS_STATES_T state_index;

/* Motor controller target parameters */
int16_t current_speed; 
int16_t speed_target;
float speed_target_percent = 0.0;
int32_t speed_target_scaled;

/* Wireless Communication parameters */
/*************************************************************************
* Expected Messages: 
* No button                   0
* Left button:                1
* Right button:               2
* Both button:                3
* Patient emergency button:   4
*************************************************************************/
char msg[6];
uint8_t msg_len = 6;

// /*************************************************************************
// * Initialize Local Variables
// *************************************************************************/ 
smc = SMC(RPM_POTENTIOMETER);
fsm = FSM(SYS_INIT);
test_start_time = 0;  
test_target_time = 0;
test_time_elapsed = 0;
state_index = SYS_INIT;
current_speed = 0;
speed_target = 0;
chair_stop_timestamp = 0;
end_of_test_timestamp = 0;
chair_stop_flag = 0;

// /*************************************************************************
// * Initialize Bluetooth Library
// *************************************************************************/
  Bluefruit.begin(0, 1);
  
  Bluefruit.setName("Controller Central");

  // Configure Battyer client
  clientBas.begin();  

  // Configure DIS client
  clientDis.begin();

  // Init BLE Central Uart Serivce
  clientUart.begin();
  clientUart.setRxCallback(bleuart_rx_callback);

  // Increase Blink rate to different from PrPh advertising mode
  Bluefruit.setConnLedInterval(250);

  // Callbacks for Central
  Bluefruit.Central.setConnectCallback(connect_callback);
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);

  /* Start Central Scanning
   * - Enable auto scan if disconnected
   * - Interval = 100 ms, window = 80 ms
   * - Don't use active scan
   * - Start(timeout) with timeout = 0 will scan forever (until connected)
   */
  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0);                   // // 0 = Don't stop scanning after n seconds

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
// sys_display_init(&lcd);
// while(true){
//   Serial.println(smc.controlPot());
//   delay(500);
// }

while(1){
switch (fsm.getCurrentState())
{
case SYS_INIT:
  Serial.println("NASA Vestibular Chair -- Loading interface...");

  /* Initialize I/O Pins */
  sys_io_init();

  /* Initialize motor controller & interface */
  Wire.begin(); 
  Wire.setClock(400000);
  smc.exitSafeStart();

  /* Initialize displays */
  sys_display_init(&lcd);

  /* Initialize web interface */

  /* Initialize communication interface */
  if ( !Bluefruit.Central.connected() )
  {
    Serial.println("init failed");
    for (uint8_t i = 0; i < sizeof(msg); i++)
    {
      msg[i] = 0;
    }
  }

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
    //   Serial.print("speed_target_scaled: ");
    // Serial.println(speed_target_scaled);
  if(speed_target_scaled < 12)
  {
    speed_target_scaled = 12;
  }

  if(speed_target_scaled > 22)
  {
    speed_target_scaled = 22;
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

  // Serial.print("\tTime: ");
  // Serial.print("speed_target_scaled: ");
  // Serial.println(test_target_time);
  break;

case SYS_ARM_HOLD:
  /*************************************************************************
  * Check for new file/patient signal
  *************************************************************************/
  if (!strstr(msg, "START") && start_test_flag == 0)
  {
    strcpy(msg, rx);
    // receiver.recv(msg, &msg_len);

    if(strstr(msg, "START"))
    {
      Serial.println("START"); /* Send start of file signal */
        for (uint8_t i = 0; i < sizeof(msg); i++)
  {
    msg[i] = 0;
  }
  start_test_flag = 1;
  
    }
  }

  lcd.setCursor(strlen("Status: "), 2);
  /* Return to previous state */
  if (digitalRead(REJECT_BTN) == LOW)
  {
    lcd.print("        ");
    lcd.setCursor(strlen("Status: "), 2);
    lcd.print("DISARMED");
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
    lcd.print("DISARMED");
    state_index = SYS_ARM_HOLD;
    digitalWrite(DISARMED_LED, HIGH);
    digitalWrite(ARMED_LED, LOW);
  }else if(digitalRead(ARMING_SWITCH_1) == LOW){
    lcd.print("        ");
    lcd.setCursor(strlen("Status: "), 2);
    lcd.print("ARMED");
    digitalWrite(DISARMED_LED, LOW);
    digitalWrite(ARMED_LED, HIGH);
  
   if(digitalRead(CONFIRMATION_BTN) == LOW) {
    lcd.print("       ");
    lcd.setCursor(strlen("Status: "), 2);
    /* Wait until button depress */
    while(digitalRead(CONFIRMATION_BTN) == LOW);
    state_index = ARMED_OPERATION;
    lcd.print("ARMED_OP");
    for (uint8_t i = 0; i < sizeof(msg); i++)
  {
    msg[i] = 0;
  }
  *msg = 0;
  }

  }
  
  break;

case ARMED_OPERATION:
  test_start_time = millis();
  test_time_elapsed = millis();
  test_target_time = test_target_time*1000; /* Convertfrom S to MS */

  lcd.setCursor(0,3);
  lcd.print("Time left: ");

 while(state_index == ARMED_OPERATION)
 {
  /*********************************************************************
  * Emergency Stop Detected --> Stop Test Immediately
  **********************************************************************/
  if(digitalRead(ESTOP_BTN) == LOW)
  {
    emergency_stop();
    Serial.println("EOF");
    sys_display_init(&lcd);
    state_index = SYS_INIT;
    Serial.println((char*)msg);

   #if 0 /* TO be used in next revision */
      /* Speed */
   Serial.print("Speed Target: ");
   Serial.print(speed_target_scaled);
   Serial.println("RPM ");

   /* Duration */
   Serial.print("Spin duration: ");
   Serial.print(test_target_time);
   Serial.println("ms ");

   /*Chair stop time */ 
   Serial.print("Chair stops at: ");
   Serial.print(chair_stop_timestamp);
   Serial.println("ms ");

   /* When the test ends/button is pressed */
   Serial.print("Test stopped at ");
   Serial.print(end_of_test_timestamp);
   Serial.println("ms ");
   
   /* End of file signal */
   Serial.println("EOF");
   #endif
    
    break;
  }

  /**********************************************************************
  * Capture patient data from wireless device
  * and send to PC
  * 
  **********************************************************************/
  if (clientUart.discovered()) // Non-blocking
  {
  strcpy(msg, rx);
  int i;
  // *msg = 0;multi_btn_signalp
  // Serial.println("DATA_LOG:");
  Serial.print((test_time_elapsed - test_start_time));
  // delay(500);
  Serial.print(",");
  // delay(500);
  Serial.println((char*)msg);
  }
  // data_log[msg_index] = (test_time_elapsed - test_start_time) + "," + (char*)msg;
  // Serial.println(data_log[msg_index]);


  /*********************************************************************
  * Gradually increase RPM until target is reached
  **********************************************************************/
  if(test_target_time+1000 > (test_time_elapsed - test_start_time))
  {
    if(current_speed < speed_target)
    {
    smc.setMotorSpeed(current_speed);
    current_speed+=4;
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
 lcd.setCursor(0,2);
 lcd.print("1. Press yllw btn ");
 lcd.setCursor(0,3);
 lcd.print("2. Press green btn");
 while(true)
 {
    test_time_elapsed = millis();

    if(current_speed > 0)
    {
    current_speed-=25;
    // state_index = ARMED_TARGET_REACHED;
    smc.setMotorSpeed(current_speed);
    // delay(200);
    }

    /**********************************************************************
    * NOTE: Repurposing emergency stop switch to mark when chair has 
    * stopped
    **********************************************************************/ 
    if(digitalRead(ESTOP_BTN) == LOW && chair_stop_flag == 0) 
    {
      chair_stop_timestamp = (test_time_elapsed - test_start_time);
      chair_stop_flag = 1;
    }

    /**********************************************************************
    * Capture patient data from wireless device
    * and send to PC
    **********************************************************************/
    if(clientUart.discovered())
    {
    strcpy(msg, rx);
      int i;
    // *msg = 0;
    // Serial.println("DATA_LOG:");
    Serial.print((test_time_elapsed - test_start_time));
    // delay(500);
    Serial.print(",");
    // delay(500);
    Serial.println((char*)msg);
    }

    
    /**********************************************************************
    * Finish logging data when proctor has confirmed patient signal
    * that chair has stopped
    **********************************************************************/
   if (digitalRead(CONFIRMATION_BTN) == LOW)
   {
    /* code */
   end_of_test_timestamp = (test_time_elapsed - test_start_time);

    lcd.setCursor(0,3);
    lcd.print("Saving data to file");
    /* Insert end of file signal */
    Serial.println("EOF");

    /**********************************************************************
    * Log system events
    **********************************************************************/
   /* Speed */
   Serial.print("Speed Target: ");
   Serial.print(speed_target_scaled);
   Serial.println("RPM ");

   /* Duration */
   Serial.print("Spin duration: ");
   Serial.print(test_target_time);
   Serial.println("ms ");

   /*Chair stop time */ 
   Serial.print("Chair stops at: ");
   if(chair_stop_flag){
   Serial.print(chair_stop_timestamp);
   Serial.println("ms ");
   }else{
    Serial.print("N/A ");
   }

   /* When the test ends/button is pressed */
   Serial.print("Test stopped at ");
   Serial.print(end_of_test_timestamp);
   Serial.println("ms ");
   
   /* End of file signal */
   Serial.println("EOF");
   
   delay(4000);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Done");
   delay(3000);



  /**********************************************************************
  * Reset for next sequence
  **********************************************************************/
 sys_display_init(&lcd);
 *msg = 0;
 start_test_flag = 0;
 state_index = SYS_INIT;
 chair_stop_flag = 0;/* Reset flag */
 break;
   }
}

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
// Serial.print("Current State index: ");
// Serial.print(fsm.getCurrentState());
// Serial.print("\t");
// Serial.print("Next state index: ");
// Serial.println(state_index);
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
  start_test_flag = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("EMERGENCY STOP");
    smc.eStop();
    delay(2000);
    fsm.setCurrentState(SYS_INIT);
}

/*************************************************************************
* Function: Bluetooth Callbacks
*
* Purpose: used to connect and receive data 
*************************************************************************/

/**
 * Callback invoked when scanner pick up an advertising data
 * @param report Structural advertising data
 */
void scan_callback(ble_gap_evt_adv_report_t* report)
{
  // Check if advertising contain BleUart service
  if ( Bluefruit.Scanner.checkReportForService(report, clientUart) )
  {
    Serial.print("BLE UART service detected. Connecting ... ");

    // Connect to device with bleuart service in advertising
    Bluefruit.Central.connect(report);
  }else
  {      
    // For Softdevice v6: after received a report, scanner will be paused
    // We need to call Scanner resume() to continue scanning
    Bluefruit.Scanner.resume();
  }
}

/**
 * Callback invoked when an connection is established
 * @param conn_handle
 */
void connect_callback(uint16_t conn_handle)
{
  Serial.println("Connected");

  Serial.print("Dicovering Device Information ... ");
  if ( clientDis.discover(conn_handle) )
  {
    Serial.println("Found it");
    char buffer[32+1];
    
    // read and print out Manufacturer
    memset(buffer, 0, sizeof(buffer));
    if ( clientDis.getManufacturer(buffer, sizeof(buffer)) )
    {
      Serial.print("Manufacturer: ");
      Serial.println(buffer);
    }

    // read and print out Model Number
    memset(buffer, 0, sizeof(buffer));
    if ( clientDis.getModel(buffer, sizeof(buffer)) )
    {
      Serial.print("Model: ");
      Serial.println(buffer);
    }

    Serial.println();
  }else
  {
    Serial.println("Found NONE");
  }

  Serial.print("Dicovering Battery ... ");
  if ( clientBas.discover(conn_handle) )
  {
    Serial.println("Found it");
    Serial.print("Battery level: ");
    Serial.print(clientBas.read());
    Serial.println("%");
  }else
  {
    Serial.println("Found NONE");
  }

  Serial.print("Discovering BLE Uart Service ... ");
  if ( clientUart.discover(conn_handle) )
  {
    Serial.println("Found it");

    Serial.println("Enable TXD's notify");
    clientUart.enableTXD();

    Serial.println("Ready to receive from peripheral");
  }else
  {
    Serial.println("Found NONE");
    
    // disconnect since we couldn't find bleuart service
    Bluefruit.disconnect(conn_handle);
  }  
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;
  
  Serial.println("Disconnected");
}

/**
 * Callback invoked when uart received data
 * @param uart_svc Reference object to the service where the data 
 * arrived. In this example it is clientUart
 */
void bleuart_rx_callback(BLEClientUart& uart_svc)
{
  // Serial.print("[RX]: ");
  int i = 0;
  char buf[6];
  
  while ( uart_svc.available() )
  {
    // Serial.print( (char) uart_svc.read() );
    buf[i] = (char) uart_svc.read();
    i++;
    if(i>5){
      i = 0;
    }
  }
  strcpy(rx,buf);

  // Serial.println();
}
/*************************************************************************
* Function: sys_init
*
* Purpose: Initializes system peripherals and objects 
*************************************************************************/
// void sys_init()
// {

// }
