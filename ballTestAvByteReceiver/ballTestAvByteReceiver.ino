#include <Ball2.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball2 ball;
byte firstByte;
byte rB = 0;
byte gB = 0;
byte bB = 0;

void setup(){
  Serial.begin(19200);
  ball.setColor(0,0,0);
}


void loop(){
}

void serialEvent(){
  int dummy;
  if(Serial.available() >= 4){
    firstByte = Serial.read();
    if (firstByte == 'c') {
      int rB = Serial.read();
      int gB = Serial.read();
      int bB = Serial.read();
      ball.setColor(rB, gB, bB);
      while (Serial.available()){
        dummy = Serial.read();
      }
    }
  }
}




