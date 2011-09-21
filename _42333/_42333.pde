#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;
static unsigned char r = 0, g = 0, b = 0;
static int beatCounter = 0;

void setup(){
  Serial.begin(19200);
  
  MsTimer2::set(5, processAD); // 5ms period, 200Hz
  //MsTimer2::set(100, broadcast); // 100ms period, 10Hz
  MsTimer2::start();
  // Testing
   ball.setColor(1,0,0);
}

void processAD(){
    ball.processAD();
    ball.setReleaseColor(r,g,b);
   beatCounter++;
}

void loop(){
  static int flyTid, prevFlyTid, high, temp, flyTeller;
  static int i = 0,diff;
  
  if(ball.getLanded()){
     ball.resetLanded();
     
      prevFlyTid = flyTid;
     flyTid = ball.getFlyTime();
    
    diff = flyTid - prevFlyTid;
    
 
   if(diff < -25){
      r = 255;
      g = 100;
      b = 0;
      high = 1;

   }else{
    r = 0;
    g = 128;
    b = 128;
   }

    Serial.print(' ');
    Serial.print(beatCounter);
    Serial.print(' ');
    Serial.print('A');
    


}
  
   if(Serial.available()){
    Serial.flush();
    beatCounter=0;
  }
}

