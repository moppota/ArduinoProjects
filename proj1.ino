// Code to make a joystick control digits on a seven segment display, along with the colour of an RGB LED
#define VRX_PIN A1
#define VRY_PIN A0
#include "SevSeg.h"

SevSeg sevseg;
//init variables 
int xValue = 0;
int yValue = 0;//joystick

int Number = 5000;//starting segment number

int rgbRED = 7;//rgb led pins
int rgbGREEN = 6;
int rgbBLUE = 5;

void setup(){
  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {24, 26, 35, 39, 41, 22, 33, 37};//init pins

  pinMode(rgbRED, OUTPUT);
  pinMode(rgbGREEN, OUTPUT);
  pinMode(rgbBLUE, OUTPUT);

  bool resistorsOnSegments = true; 
  bool updateWithDelaysIn = true;
  byte hardwareConfig = COMMON_CATHODE; 

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);//begin sevseg display

  analogWrite(rgbRED, 124);
  analogWrite(rgbBLUE,124); //turn on rgb led

  Serial.begin(9600);
  Serial.println("Begun");
}

void loop(){
    yValue = analogRead(VRY_PIN);
    Number = Number + round(((499 + -(yValue)) / 100)); //simple way of making the number increase faster as you move the joystick further in one direction

    int rgbNumber = Number / 40;
    analogWrite(rgbGREEN, rgbNumber); //change rgb led to reflect number, Lower number goes white, Higher number goes lime

    sevseg.setNumber(Number, 0);
    sevseg.refreshDisplay(); //refresh sevseg display
}