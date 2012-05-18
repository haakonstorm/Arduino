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
   ball.setColor(4,0,0);
}

static int beatCounter = 0;

void processAD(){
    ball.processAD();
    beatCounter++;
}


void loop(){
  static int pos = 0;
  static int beatSum = 0;
  static int beatMean = 0;
  
  if(ball.getLanded()){
     ball.resetLanded();
     
   
  }
  
  if(Serial.available()){
    Serial.flush();
    beatCounter=0;
  }

}




