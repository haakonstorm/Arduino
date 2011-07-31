#include <MsTimer2.h>
#include <EEPROM.h>
#include <Ball.h>


Ball ball;


void setup(){
  MsTimer2::set(5, readAD); // 5ms period, fSample = 200Hz. (too fast?)
  MsTimer2::start();
}

void readAD(){
  ball.processAD();
}

void loop(){

}

