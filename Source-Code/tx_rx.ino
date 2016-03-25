/*Gateway
  Acts as a transceiver. Receives the data from the 
  a group of Smart bins and transmits the data to the web server.*/



#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

int msg[1];

RF24 radio(9,10);// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 


const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL };// Radio pipe addresses for the  2 nodes to communicate.

String theMessage = "";
String theMsg = "";
int st=0;
char a11;
int charToSend[1];

void setup()
{
 Serial.begin(9600);
 radio.begin();
 delay(500);
}

void loop()
{
 radio.openReadingPipe(1,pipes[0]);
 radio.startListening();
 
 while(st==0)
 {
  if (radio.available())
  {
   bool done = false;  
   done = radio.read(msg, 1); 
   char theChar = msg[0];
   if (msg[0] != 2)
   {
    theMessage.concat(theChar);
    a11=theChar;
   }
   if(a11=='#')
   {  
    Serial.println(theMessage);
    theMsg = theMessage;
    theMessage= ""; 
    st=1;
   }
  }
 }
 radio.stopListening();
 radio.openWritingPipe(pipes[1]);

 int messageSize = theMsg.length();
 for (int i = -1; i < messageSize; i++) 
 {
  charToSend[0] = theMsg.charAt(i);
  radio.write(charToSend,1);
 }   
 msg[0] = 2; 
 radio.write(msg,1);
 st=0;
 delay(1);
}
