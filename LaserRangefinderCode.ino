/*!
 * @File  DFRobot_Iraser_Sensor.ino
 * @brief  In this example, the infrared laser ranging sensor is used to measure the distance, and the sensor data is processed to obtain the measured distance
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence  The MIT License (MIT)
 * @author  [liunian](nian.liu@dfrobot.com)
 * @version  V1.0
 * @date  2020-08-13
 */

 // this code is not made by me, however, it actually works with my mega board so Im storing it here
#define ADDR 0x80
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10,11);//Define software serial, 3 is TX, 2 is RX
char buff[4]={ADDR,0x06,0x03,0x77}; //continuous measurement Request
// Continuous measurement returns 11 Bytes. On success:
// ADDR 0x06 0x83 0x3X 0x3X 0x3X 0x2E 0x3X 0x3X 0x3X CHECK_BYTE (where 0x3X are Ascii digits, 0x2E is Ascii '.')
// On Fail:
// ADDR 0x06 0x83 'E' 'R' 'R' '-' '-' '-' 0x3X CHECK_BYTE (where 3X are Ascii digits, 0x2E is Ascii '.')
unsigned char data[11]={0}; //prepate an array to receive data
void setup()
{
 Serial.begin(115200); //Hardware serial, for use with serial monitor
 mySerial.begin(9600); //Software serial, for use with Laser
 
 delay(50); //delays just because DFRobot did, to give the laser sometime to respond
 
 const unsigned char SetAddr[5] = {0xFA, 0x04, 0x01, ADDR, 0x81}; //set the address to 0x80, just in case
 mySerial.write(SetAddr, 5);
 delay(50);
 while(mySerial.available()){
   Serial.write(mySerial.read()); //pass any returned data to the serial monitor (success = FA 04 81 81, fail = FA 84 81 02 FF)
 }
 delay(50);
}

void loop()
{
  mySerial.write(buff, 4); // Request continous measurement mode
  while(1)
  {
    if(mySerial.available()>0) //Determine whether there is data to read from the Laser
    {
      delay(50);
      //read received data
      for(int i=0;i<11;i++)
      {
        data[i]=mySerial.read();
      }
      //calculate the CHECK_BYTE for received data
      unsigned char Check=0;
      for(int i=0;i<10;i++)
      {
        Check=Check+data[i];
      }
      Check=~Check+1;
      if(data[10]==Check) //Compare received CHECK_BYTE to calculated CHECK_BYTE
      { //If the check_bytes match, data is ok
        if(data[3]=='E'&&data[4]=='R'&&data[5]=='R')
        {
          Serial.println("Out of range");
        }
        else
        {
          float distance=0;
          distance=(data[3]-0x30)*100+(data[4]-0x30)*10+(data[5]-0x30)*1+(data[7]-0x30)*0.1+(data[8]-0x30)*0.01+(data[9]-0x30)*0.001; //Convert the distance from a string of chars to a float.
          Serial.print("Distance = ");
          Serial.print(distance,3); //Print the value to Hardware Serial
          Serial.println(" M");
        }
      }
      else //The CHECK_BYTEs don't match. Something's wrong!
      {
        Serial.println("Invalid Data!");
      }
    }
    delay(20);
  }
}
