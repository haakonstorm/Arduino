#include <EEPROM.h>
#include <Ball.h>
#include <MsTimer2.h>

Ball ball;

void setup(){
  analogReference(EXTERNAL);

  //øk frekvense på pwm: se http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1235060559
  TCCR0B = TCCR0B & 0b11111000 | 0x02;
  TCCR1B = TCCR1B & 0b11111000 | 0x02;

  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  Serial.begin(19200);


  MsTimer2::set(15, readAD); // 5ms period, fSample = 200Hz. (too fast?)
  MsTimer2::start();
}

void readAD(){
  ball.processAD();
}

void loop(){

}

