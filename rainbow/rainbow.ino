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

void processAD(){
  counter++;
  if (counter % 2 == 0) {
    turnOn();
  } else {
    turnOff();
  }
}

void turnOn() {
  ball.setColor(255,0,0);
}

void turnOff() {
  ball.setColor(0,0,0);
}


void loop() {
  
  ball.setColor(255,0,0); // red
  delay(5);
  ball.setColor(0,0,0);
  delay(95);
  
  ball.setColor(255,128,0); // orange
  delay(5);
  ball.setColor(0,0,0);
  delay(95);
  
  ball.setColor(255,255,0); // yellow
  delay(5);
  ball.setColor(0,0,0);
  delay(95);
  
  ball.setColor(0,255,0); // green
  delay(5);
  ball.setColor(0,0,0);
  delay(95);
  
  ball.setColor(0,0,255); // blue
  delay(5);
  ball.setColor(0,0,0);
  delay(95);
  
  ball.setColor(111,0,255); // indigo
  delay(5);
  ball.setColor(0,0,0);
  delay(95);
  
  ball.setColor(143,0,255); // orange
  delay(5);
  ball.setColor(0,0,0);
  delay(95);
  
  
  
}

