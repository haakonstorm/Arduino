#include <EEPROM.h>
#include <Ball.h>
#include <MsTimer2.h>

Ball ball;

void setup(){
  MsTimer2::set(5, ball.readAD); // 5ms period, fSample = 200Hz. (too fast?)
  MsTimer2::start();

}
void loop(){
  
}
