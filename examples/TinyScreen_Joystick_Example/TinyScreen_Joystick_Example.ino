/************************************************************************
 * This tutorial shows how the TinyShield Joystick board's values can be
 * read using a TinyScreen+ processor board. The TinyScreen+ also displays
 * graphics that prove the joystick outputs are being read properly.
 *
 * Hardware by: TinyCircuits
 * Written by: Tony Batey for TinyCircuits
 *
 * Initiated: Mon. 11/20/2015 
 * Updated: Tue. 04/08/2019
 ************************************************************************/

// Libraries
#include <TinyScreen.h>
#include <SPI.h>
#include <Wire.h>

// TinyScreen+ variables
TinyScreen display = TinyScreen(TinyScreenPlus);
int background = TS_8b_Black;

// Color constants
#define	BLACK           0x00
#define	BLUE            0xE0
#define	RED             0x03
#define	GREEN           0x1C
#define WHITE           0xFF
#define	GREY            0x6D

// Global variables to hold joystick data
int RX=0;
int RY=0;
int LX=0;
int LY=0;
byte leftButton=0;
byte rightButton=0;

#if defined(ARDUINO_ARCH_SAMD)
 #define SerialMonitorInterface SerialUSB
#else
 #define SerialMonitorInterface Serial
#endif

// Begin Serial, 
void setup(void) {
  Wire.begin();
  SerialMonitorInterface.begin(9600);
  display.begin();
}

void loop() {
  getJoystick();
  
  byte color;
  byte activeColor;
  
  if(leftButton)activeColor=RED;else activeColor=GREEN;
  if(LY>200)color=activeColor;else color=GREY;
  drawUpArrow(24,8,10,color);
  if(LY<-200)color=activeColor;else color=GREY;
  drawDownArrow(24,56,10,color);
  if(LX>200)color=activeColor;else color=GREY;
  drawRightArrow(47,32,10,color);
  if(LX<-200)color=activeColor;else color=GREY;
  drawLeftArrow(1,32,10,color);
  if(abs(LY)<200 && abs(LX)<200)color=activeColor;else color=GREY;
  drawFilledCircle(24, 32, 8, color);
  
  if(rightButton)activeColor=RED;else activeColor=GREEN;
  if(RY>200)color=activeColor;else color=GREY;
  drawUpArrow(24+48,8,10,color);
  if(RY<-200)color=activeColor;else color=GREY;
  drawDownArrow(24+48,56,10,color);
  if(RX>200)color=activeColor;else color=GREY;
  drawRightArrow(47+48,32,10,color);
  if(RX<-200)color=activeColor;else color=GREY;
  drawLeftArrow(1+48,32,10,color);
  if(abs(RY)<200 && abs(RX)<200)color=activeColor;else color=GREY;
  drawFilledCircle(24+48, 32, 8, color);
  
  delay(50);
  
}


/****************************** Various Drawing Functions ***********************************/

void drawFilledCircle(int x0, int y0, int radius, uint8_t color)
{
  int x = radius;
  int y = 0;
  int radiusError = 1-x;
 
  while(x >= y)
  {
    display.drawLine(x0+x,y0+y,x0-x,y0+y,color);
    display.drawLine(x0+y,y0+x,x0-y,y0+x,color);
    display.drawLine(x0-x,y0-y,x0+x,y0-y,color);
    display.drawLine(x0-y,y0-x,x0+y,y0-x,color);
    y++;
    if (radiusError<0)
    {
      radiusError += 2 * y + 1;
    }
    else
    {
      x--;
      radiusError += 2 * (y - x) + 1;
    }
  }
}

void drawDownArrow(int x, int y, int height, byte color){
  for(int i=0;i<=height;i++){
    display.drawLine(x-i,y-i,x+i,y-i,color);
    delayMicroseconds(100);
  }
}

void drawUpArrow(int x, int y, int height, byte color){
  for(int i=0;i<=height;i++){
    display.drawLine(x-i,y+i,x+i,y+i,color);
    delayMicroseconds(100);
  }
}

void drawRightArrow(int x, int y, int height, byte color){
  for(int i=0;i<=height;i++){
    display.drawLine(x-i,y+i,x-i,y-i,color);
    delayMicroseconds(100);
  }
}

void drawLeftArrow(int x, int y, int height, byte color){
  for(int i=0;i<=height;i++){
    display.drawLine(x+i,y+i,x+i,y-i,color);
    delayMicroseconds(100);
  }
}


// Reading joystick values, storing in global variables
void getJoystick(){
  Wire.requestFrom(0x22,6);
  int data[4];
  for(int i=0;i<4;i++){
    data[i]=Wire.read();
  }
  byte lsb=Wire.read();
  byte buttons=~Wire.read();
  leftButton=buttons&4;
  rightButton=buttons&8;
  for(int i=0;i<4;i++){
    data[i]<<=2;
    data[i]|= ((lsb>>(i*2))&3);
    data[i]-=511;
  }
  RX=data[0];
  RY=-data[1];
  LX=-data[2];
  LY=data[3];
}
