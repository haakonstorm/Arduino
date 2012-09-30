
#include <Ball2.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball2 ball;
int force, simpleColor;

void setup(){
  checkBattery();
  MsTimer2::set(5, processAD);
  MsTimer2::start();
}

void loop() {
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
    
    force = ball.getForceIntegral();
    simpleColor = map(constrain(force, 6000, 12000), 6000, 12000, 150, 255);
    
    ball.setSimpleColor((byte) simpleColor);
    ball.setTargetBrightness(255);
    ball.setFadeSpeed(7);
  }
}

void checkBattery() {
  int flashes;
  float batteryLevel = ball.getBatteryLevel();
  if(batteryLevel>4.0f)
    flashes=6;
  else if (batteryLevel>3.95f)
    flashes=5;
  else if (batteryLevel>3.90f)
    flashes=4;
  else if (batteryLevel>3.80f)
    flashes=3;
  else if (batteryLevel>3.70f)
    flashes=2;
  else
    flashes=1;

  for(int i = 0;i<flashes;i++){
    ball.setColor(0,10,0);
    delay(50);
    ball.setColor(0,0,0);
    delay(100);
  }  
}

void sendCommand(byte A, byte B, byte C, byte D, byte E) {
  Serial.write(2);
  Serial.write(A);
  Serial.write(B);
  Serial.write(C);
  Serial.write(D);
  Serial.write(E);
  Serial.write(3);
}




