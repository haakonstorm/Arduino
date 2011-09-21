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
   ball.setColor(250,0,2);
}

int summen =0;
int blinkNaa = 0;

int flyTeller = 0;
void processAD(){
    ball.processAD();
   if(ball.getInAir()){
      flyTeller++;
      if((flyTeller-31) == blinkNaa){
        ball.setColor(0,0,30);
      }else if((flyTeller-15) == blinkNaa){
        ball.setColor(0,0,60);
         }else if((flyTeller-8) == blinkNaa){
        ball.setColor(0,0,220);
         }else if(flyTeller == blinkNaa){
        ball.setColor(0,0,255);
         }else if((flyTeller+8) == blinkNaa){
        ball.setColor(0,0,220);
         }else if((flyTeller+15) == blinkNaa){
        ball.setColor(0,0,60);
         }else if((flyTeller+30) == blinkNaa){
        ball.setColor(0,0,30);
         }else if((flyTeller-31)== blinkNaa){
        ball.setColor(0,0,0);
      } 
   }else{
      flyTeller = 0; 
   }
}


void loop(){
  
 // if(ball.getInAir())
  //{
  // ball.setColor(255,0,0); 
    
 // }else{
  //  
 // }
  
  static int flyTider[2];
  static int i;
  if(ball.getLanded()){
     ball.resetLanded();
     flyTider[i] = ball.getFlyTime();
     i++;
     if(i>1){
       i=0;
     }
     summen = flyTider[0]+flyTider[1];
     blinkNaa = (int)summen/4;
  }
}



