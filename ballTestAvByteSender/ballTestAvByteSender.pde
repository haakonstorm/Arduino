#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;
static byte r = 65;
static byte g = 65;
static byte b = 65;

void setup(){
  Serial.begin(19200);
  MsTimer2::set(30, sendColor);
  MsTimer2::start();
  ball.setColor(0,0,0);
}

void sendColor(){
  Serial.write('C');
  Serial.write(r);
  Serial.write(g);
  Serial.write(b);
  incR();
  incG();
  incB();
}

void incR() {
  if (r >= 255) {
    r=0;
  } 
  else {
    r=r+1;
  }
}

void incG() {
  if (g >= 255) {
    g=0;
  } 
  else {
    g=g+2;
  }
}

void incB() {
  if (b >= 255) {
    b=0;
  } 
  else {
    b=b+3;
  }
}

void loop(){
}



