#define pi= 3.1415;
unsigned long t1=0;
// w = 2 * pi* n /60 
// long int t1 = millis();

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a angle positon (0 - 360):
  float position = sensorValue * (360 / 1023.0) ;
  Serial.println(position);


  unsigned long t2 = millis();
  float speed = 2* pi  * 360 * 100 / (t1- t2); // speed in meters/hour
  t1 = t2;
  Serial.println(speed);



}
