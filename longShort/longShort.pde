#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;

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
   
}


void loop(){
  
 // if(ball.getInAir())
  //{
  // ball.setColor(255,0,0); 
    
 // }else{
  //  
 // }
  

  
  
static int flyTid, prevFlyTid, high, temp, flyTeller;
  
  static int i,diff;
  if(ball.getLanded()){
     ball.resetLanded();
     prevFlyTid = flyTid;
     flyTid = ball.getFlyTime();
    
    if (flyTid>prevFlyTid){
      ball.setColor(255,0,0);
      high=0;
   }else{
     ball.setColor(0,0,255);
     high=1;
     temp = (int)prevFlyTid/2;
   }
  }

 if(ball.getInAir()){
      flyTeller++;
      if(flyTeller == temp){
        ball.setColor(0,255,0);
      }
 }
      

}



