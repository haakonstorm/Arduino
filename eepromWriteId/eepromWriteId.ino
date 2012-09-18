#include <Ball2.h>
#include <EEPROM.h>

Ball2 ball;
unsigned char id_TOEEPROM = 10;

void setup() {
  ball.setColor(255,0,0);
  EEPROM.write(ID,id_TOEEPROM);
  if(id_TOEEPROM == EEPROM.read(ID)) {
    ball.setColor(0,255,0);
  }
}

void loop() {
}
