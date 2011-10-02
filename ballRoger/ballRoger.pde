#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;

void setup(){
  Serial.begin(19200);
  MsTimer2::set(5, processAD);
  MsTimer2::start();
  ball.setColor(255,128,0);
}

void processAD(){
}

void loop(){
}


