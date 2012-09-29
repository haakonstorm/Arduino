
#include <Ball2.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball2 ball;

void setup(){
  MsTimer2::set(5, processAD);
  MsTimer2::start();
}

void processAD(){
  ball.processAD();

  if(ball.getLanded()){
    ball.resetLanded();
    ball.setTargetBrightness(1);
    ball.setFadeSpeed(1);
  }

  if(ball.getThrown()){
    ball.resetThrown();
    
    int force = ball.getForceIntegral();
    int value = map(constrain(force, 6000, 12000),6000,12000,150,255);
    
    ball.setSimpleColor((byte) value);
    ball.setTargetBrightness(255);
    ball.setFadeSpeed(7);
  }
}

void loop() {
}
