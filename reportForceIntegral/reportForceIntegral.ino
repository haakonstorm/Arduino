
#include <Ball2.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball2 ball;

int counter;

void setup(){
  Serial.begin(19200);
  MsTimer2::set(5, processAD);
  MsTimer2::start();
  ball.setColor(3,3,3);
  ball.setFadeSpeed(10);
}

void processAD(){
  ball.processAD();
  counter++;
}

void loop(){

  if(ball.getLanded()){
    ball.resetLanded();
    ball.setSiteSwapColor(3);
  }

  if(ball.getThrown()){
    ball.resetThrown();
    ball.setSiteSwapColor(2);
    Serial.print(ball.getHoldTime());
    Serial.print(" ");
    Serial.println(ball.getForceIntegral());
  }
}


