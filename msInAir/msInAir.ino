
#include <Ball2.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball2 ball;

void setup(){
  Serial.begin(19200);
  MsTimer2::set(5, processAD);
  MsTimer2::start();
  ball.setColor(255,128,0);
}

void processAD(){
   ball.processAD();
}

void loop(){
  if(ball.getLanded()){
    Serial.println(ball.getHoldTime());
    ball.resetLanded();
    setColorSS(0);
  }

  if(ball.getThrown()){
    setColorSS(2);
    ball.resetThrown();
  }
}


void setColorSS(int i) {
  switch (i) {
  case 0:
    ball.setColor(1,1,1); // DIM
    break;
  case 1:
    ball.setColor(255,0,0); // RED
    break;
  case 2:
    ball.setColor(255,255,0); // YELLOW
    break;
  case 3:
    ball.setColor(0,255,0); // GREEN
    break;
  case 4:
    ball.setColor(0,0,255); // BLUE
    break;
  case 5:
    ball.setColor(128,128,255);
    break;
  case 6:
    ball.setColor(128,255,128);
    break;
  case 7:
    ball.setColor(255,128,128);
    break;
  case 8:
    ball.setColor(255,255,255);
    break;
  case 9:
    ball.setColor(255,0,255);
    break;    
  default:
    ball.setColor(1,1,1); // DIM
    break;
  }
}
