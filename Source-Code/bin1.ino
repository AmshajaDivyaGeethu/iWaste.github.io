/*Smart Bin 
  Combination of Ultrasonic sensor,DHT11 sensor,Gas sensor 
  which is integrated with the nRF module. A servo motor is 
  used to open and close the lid of the Bin when needed.   */



#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <dht.h>

#define dht_apin 7 // sensor is connected to 7

#define trigPin1 5//Fill-level sensor trigger pin
#define echoPin1 6//Fill-level sensor echo pin

#define trigPin2 3//Proximity sensor trigger pin
#define echoPin2 4//Proximity sensor echo pin

dht DHT;

RF24 radio(9, 53);// Set up nRF24L01 radio on SPI bus plus pins 9 & 53 

int msg[1];
int messageSize;
const uint64_t pipe = 0xF0F0F0F0E1LL;// Radio pipe address for the  node to communicate.
int charToSend[1];
int bo = 0;

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position
long distance1; // Duration used to calculate distance
long distance2;

int sensorpin=A0;//LPG sensor
int  pin=12;
int sv=0;

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(pin, OUTPUT);
  delay(500);
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
  radio.begin();// nRF
  radio.openWritingPipe(pipe);
}

void loop()
{
 String s= "*";
 String s1="B1";
 String s2 = "M";
 
 // read the value from the sensor:
 digitalWrite(pin, HIGH);  
 delay(1000);
 sv = analogRead(sensorpin);
 
 digitalWrite(pin, LOW);           
 delay(1000);
 Serial.println(s1);
 Serial.print(s2);
 Serial.println(sv);

 DHT.read11(dht_apin);
 int hum = DHT.humidity;
 int temp = DHT.temperature;
 String s3 = "H";
 String s4 = "T";

 Serial.print(s3);
 Serial.println(hum);
 Serial.print(s4);
 Serial.println(temp); 
 
 long a2 = dist2();//returns the value of proximity sensor
 long a1 = dist1();//returns the value of fill-level sensor
 
 //Opens the lid
 if(a2<30 && bo==0 && a1>25)
 {
  openb();
  bo=1;
 }
 
 //Closes the lid
 if(a2>30 && bo==1)
 {
  closeb();
  bo=0;
 }
 
  String s5 = "F";
  String s6 = "#";
  
  Serial.print(s5);
  Serial.println(a1);
  
  /*concatenate all the strings to transmitt the data as '*B1M2T3H4F5#' 
    where B is the bin number,M is the methane level,T is the temperature,H is the humidity,F is the fill level*/ 
  String S= s + s1 + s2 + String(sv) + s3 + String(hum) + s4 + String(temp)+ s5 + String(a1) + s6;
  messageSize = S.length();
 
 for (int i = -1; i < messageSize; i++)
 {
  charToSend[0] = S.charAt(i);
  radio.write(charToSend, 1);
 }
 msg[0] = 2;
 radio.write(msg, 1);
 delay(5000); 
}

void closeb()
{
 for (pos = 60; pos < 120; pos++)
  {
   delay(100);
   myservo.write(pos);
  }
}

void openb()
{
  for (pos = 120; pos > 60; pos--)
  {
   myservo.write(pos);
   delay(100);
  }
}

long dist1()
{
 digitalWrite(trigPin1, LOW);
 delayMicroseconds(2);

 digitalWrite(trigPin1, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin1, LOW);

 long distance1 = pulseIn(echoPin1, HIGH);
 //distance = distance/58;
 distance1 = distance1*0.034/2;
 return(distance1);
}

long dist2()
{
 digitalWrite(trigPin2, LOW);
 delayMicroseconds(2);

 digitalWrite(trigPin2, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin2, LOW);

 long distance2 = pulseIn(echoPin2, HIGH);
 //distance = distance/58;
 distance2 = distance2*0.034/2;
 return(distance2);
}
