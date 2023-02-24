float value = 0;
float rev = 0;
int RPM;
int time;
float position;
float initialpos;
float RPS;
float mySensVals[4] = { 0, 0, 0, 0 };
int count = 0;
float lastpos=0;

int Sample = 4; // samples of data 
int Period = 1000/ Sample;


void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

}

// the loop routine runs over and over again forever:


void loop()
{

  int sensorValue = analogRead(A0);  // read the input on analog pin 0:
  float position = sensorValue * (360 / 1023.0);  // Convert the analog reading (which goes from 0 - 1023) to a angle positon (0 - 360):
  unsigned long t1 = millis();
  delay(50);
  
  sensorValue = analogRead(A0);  // read the input on analog pin 0:
  float position2= sensorValue * (360 / 1023.0);
  unsigned long t2 = millis();

  if (position2 < position )
  {
  position2 = position2+360;
  }
  float speed = (((position2-position) / (t2- t1) )/6 ) *1000; // speed in meters/hour
  // t1 = t2;

  Serial.print("speed:");  //saves the current time
  Serial.println(speed);

  delay(200);
}





void speed() { 

  if (lastpos==0){lastpos=position;}
  


  float change = position - lastpos; // gets a difference in position vs last position  

  mySensVals[count] = change; //stores delta into an size 4 array to average them.
  if (count == 3) {
    count = 0;
  } else {
    count++;
  }

 // Serial.println(change); // test

  float change4=0;
  for (int index = 0; index <4; index++) {
    change4 += mySensVals[index]; 
    // Serial.println(mySensVals[index]); //test 
  }
  // Serial.println("-----"); 
  // Serial.print(change4); 
  Serial.println("-----"); //spacer 

  RPS = change4 / Sample;    // averages change in rotation relative to time 
  lastpos = position;

 


  delay(Period);
  Serial.print("RPS:");  //saves the current time
  Serial.println(RPS);




  // oldposition=position;

  // Serial.println(RPM);


  // unsigned long t1 = millis();
  // dealy(50)

  // Serial.println(speed);
}



/*

//have a way to calc total revs 
//absolute encoder
//

known degrees and revolution 
have info in degrees 
need info how fast its spinning 
how to translate position and degrees and a full rev to calculate info 



*/