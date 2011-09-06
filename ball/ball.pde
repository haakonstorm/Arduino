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

int holdTime,flyTime;
void processAD(){
    ball.processAD();
    if(ball.getLanded()){
     ball.resetLanded();
    holdTime = ball.getHoldTime() * 5; 
    flyTime = ball.getFlyTime() * 5; 
    Serial.print("HoldTime: ");
    Serial.print(holdTime);
    Serial.print(". flyTime: ");
    Serial.println(flyTime);
    
    }


}


void broadcast(){
 // ball.processAD();
  
//  Serial.print ("Level: ");
//  Serial.println(ball.getBatteryLevel());
  
}

void loop(){
}



