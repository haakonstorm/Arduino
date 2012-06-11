#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;
int counter = 0;

void setup(){
//  Serial.begin(19200);
//  MsTimer2::set(50, processAD);
//  MsTimer2::start();
//  ball.setColor(255,128,0);
}


//void processAD(){
//  counter++;
//  if (counter % 2 == 0) {
//    turnOn();
//  } else {
//    turnOff();
//  }
//}

void loop() {
  ball.setColor(0,0,255);
  delay(100);
  ball.setColor(0,255,0);
  delay(100);
  ball.setColor(255,0,0);
  delay(100);
}

