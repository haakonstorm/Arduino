#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;
byte firstByte;
byte rB = 0;
byte gB = 0;
byte bB = 0;

void setup(){
  Serial.begin(19200);
  ball.setColor(0,0,0);
}


void loop(){
  if(Serial.available() >= 4){
    firstByte = Serial.read();
    if (firstByte = 'C') {
      int rB = Serial.read();
      int gB = Serial.read();
      int bB = Serial.read();
      ball.setColor(rB, gB, bB);
      Serial.flush();
    }
  }
}



