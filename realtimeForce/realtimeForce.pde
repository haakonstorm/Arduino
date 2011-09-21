#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;
static unsigned char r = 0, g = 0, b = 0;

void setup(){
  Serial.begin(19200);
  
  MsTimer2::set(5, processAD); // 5ms period, 200Hz
  //MsTimer2::set(100, broadcast); // 100ms period, 10Hz
  MsTimer2::start();
  // Testing
   ball.setColor(1,0,0);
}


static int samples = 0;
void processAD(){

  ball.processAD();
  
  if(ball.getInAir())
    {
      ball.setColor(255,0,0);
    }else{
      ball.setColor(0,255,0);
    }

   samples++;
  
}

void loop(){
    static int force;
    if(samples>=10){
      samples =0;
    force = ball.getF();
  Serial.print(' ');
    Serial.print(force);
    Serial.print(' ');
    Serial.print('A');
    }
}

