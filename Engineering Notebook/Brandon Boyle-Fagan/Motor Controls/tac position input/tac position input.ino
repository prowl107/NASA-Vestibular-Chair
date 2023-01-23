float value=0;
float rev=0;
int RPM;
int oldtime=0;
int time;
float position;


void isr() //interrupt service routine
{
rev++;
}


void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  attachInterrupt(0,isr,RISING);  //attaching the interrupt

}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  // Convert the analog reading (which goes from 0 - 1023) to a angle positon (0 - 360):
  int sensorValue = analogRead(A0);
  float oldposition = sensorValue * (360 / 1023.0) ;
  //Serial.println(position); 
  // delay(100);

// detachInterrupt(0);           //detaches the interrupt
  // position = position-oldposition; 
  time=millis()-oldtime;        //finds the time 

  RPM=(position/time)*60000;         //calculates rpm
  oldtime=millis();             //saves the current time
  // oldposition=position;

  Serial.println(RPM);

  
// attachInterrupt(0,isr,RISING);


  // unsigned long t2 = millis();
  // float speed = 2* pi  * 360 * 100 / (t1- t2); // speed in meters/hour
  // t1 = t2;
  // Serial.println(speed);



}
