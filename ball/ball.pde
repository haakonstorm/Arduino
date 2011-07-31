#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;

void setup(){
  Serial.begin(19200);
  
  MsTimer2::set(5, readAD); // 5ms period, 200Hz
  MsTimer2::start();
  
 
  
  // Testing
  // ball.setColor(100,100,0);
}


void readAD(){
  ball.processAD();

}

void loop(){
}



