#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;

void setup(){
  Serial.begin(19200);
  
  MsTimer2::set(5, readAD); // 5ms period, 200Hz
  MsTimer2::start();
  
  MsTimer2::set(100, reportWrapper); // 100ms period, 10Hz
  MsTimer2::start();
  
  // Testing
  // ball.setColor(100,100,0);
}

void reportWrapper(){
  ball.processAD();
  Serial.println(ball.sumF);
}

void readAD(){
  ball.processAD();

  if (ball.sumF > 255)
    ball.sumF = 255;

  if (ball.sumF < 15){
    ball.setColor(255,0,0);
    Serial.write('A');
  } 
  else {
    ball.setColor(0,255,0);
  }
  
  
}

void loop(){
  
}



