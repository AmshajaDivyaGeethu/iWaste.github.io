/*Admin module
  Acts as a receiver. Receives all the data send by the gateway*/


#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

#define FULL_DISTANCE 25
RF24 radio(9, 53);// Set up nRF24L01 radio on SPI bus plus pins 9 & 53 

int msg[1];
const uint64_t pipe = 0xF0F0F0F0E2LL;// Radio pipe address for the node to communicate.
String theMessage = "";
char a11;


void setup(void)
{
  Serial.begin(9600);
  Serial3.begin(9600);
  Serial2.begin(9600);

  delay(1500);
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();
}

void loop(void)
{
  if (radio.available())
  {
    bool done = false;
    done = radio.read(msg, 1);
    char theChar = msg[0];
    if (msg[0] != 2)
    {
      theMessage.concat(theChar);
      a11 = theChar;
    }
    else 
    {
      /*String newMessage = theMessage;
      Serial.println(newMessage.length());
      if (theMessage.length() >= 2) {
          Serial.println("iOF" + newMessage.indexOf('F'));*/
    }
    if (a11 == '#') 
    {
      String binNumber  = "";
      String fullAmount = "";
      int binStart = theMessage.indexOf('B');
      int binEnd;
      int fullStart;
      int fullEnd;
    //String fullValue = theMessage.substring(theMessage.indexOf("F")+1, theMessage.indexOf("#"));
      String newMessage = theMessage;
      Serial.println(newMessage.length());
      if (binStart == 1)
      {
        binEnd = theMessage.indexOf('M');
        if (binEnd != -1)
        {
          binNumber = newMessage.substring(binStart + 1, binEnd);
          Serial.print("BinNumber=");
          Serial.println(binNumber);
          fullStart = newMessage.indexOf('F');
          if (fullStart != -1)
          {
            fullEnd = newMessage.indexOf('#');
            fullAmount = newMessage.substring(fullStart + 1, fullEnd);
            Serial.print("Full amount=");
            Serial.println(fullAmount);
            int fullAmt = fullAmount.toInt();

            Serial.println(newMessage);
            Serial3.println(theMessage);

            if (fullAmt <= FULL_DISTANCE)
            {
              sms(binNumber);//Invokes sms function to send the bin numbers to the truck drivers
              
            Serial.println("sending sms");
            }
          }
        }
      }

      theMessage = "";
    }
    /*if(a11=='#')
    {
      Serial.print("RX2");
     Serial.println(theMessage);*/
    if (Serial3.available())
      Serial.write(Serial3.read());
    if (Serial.available())
      Serial3.write(Serial.read());
    //theMessage= "";
    //}
  }
}

void sms(String binNum)
{
  Serial2.print("\r");
  delay(1000);
  Serial2.print("AT\r");
  delay(1000);
  Serial2.print("AT+CMGF=1\r");
  delay(1000);
  Serial2.print("AT+CMGS=\"8891122136\"\r");
  delay(1000);
  Serial2.print("Bin Number : " + binNum + " is full\r");
  delay(1000);
  Serial2.write(0x1A);
  delay(1000);
}
