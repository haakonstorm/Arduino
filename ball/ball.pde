#include <EEPROM.h>
#include <Ball.h>
#include <MsTimer2.h>

Ball ball;

void setup(){
   MsTimer2::set(5, readAD); // 5ms period, fSample = 200Hz. (too fast?)
   MsTimer2::start();
}

void readAD(){
  ball.x = abs(analogRead(X) - xN);
  ball.y = abs(analogRead(Y) - yN);
  ball.z = abs(analogRead(Z) - zN);
  ball.processAD();
}

void loop(){
  
}
