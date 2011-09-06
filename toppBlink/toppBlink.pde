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

void processAD(){
    ball.processAD();
 
}


void broadcast(){
 // ball.processAD();
  
//  Serial.print ("Level: ");
//  Serial.println(ball.getBatteryLevel());
  
}

void loop(){
  
 // if(ball.getInAir())
  //{
  // ball.setColor(255,0,0); 
    
 // }else{
  //  ball.setColor(0,0,255); 
 // }
  
  if(ball.getLanded()){
     ball.resetLanded();
     Serial.print("l");
     ball.setColor(255,0,0); 
  }
  
  if(Serial.available() & ball.getInAir()){
      Serial.flush();
       ball.setColor(0,0,255); 
    
  }
   
}



