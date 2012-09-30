
#include <Ball2.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball2 ball;
int force, simpleColor;
int x, y, z;

void setup(){
  checkBattery();
  MsTimer2::set(5, processAD);
  MsTimer2::start();
}

void loop() {
  x = map(constrain(ball.getX(), 0, 105), 0, 105, 0, 255);
  y = map(constrain(ball.getY(), 0, 105), 0, 105, 0, 255);
  z = map(constrain(ball.getZ(), 0, 105), 0, 105, 0, 255);
  ball.setColor(x, y, z);
}

void processAD(){
  ball.processAD();
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
