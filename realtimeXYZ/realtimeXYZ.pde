#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;

void setup() {
  Serial.begin(19200);
  MsTimer2::set(5, processAD);
  MsTimer2::start();
  ball.setColor(1,1,1);
}

static int samples = 0;

void processAD() {
  ball.processAD();

  if (ball.getThrown()) {
    ball.setColor(random(255),random(255),random(255));
    ball.resetThrown();
  }

  if (ball.getLanded()) {
    ball.setColor(1,1,1);
    ball.resetLanded();
  }
  

  if (samples == 10) {
    samples = 0;
    Serial.print('m');
//    Serial.print(ball.getX());
//    Serial.print(' ');
//    Serial.print(ball.getY());
//    Serial.print(' ');
//    Serial.print(ball.getZ());
//    Serial.print(' ');
    Serial.print(ball.getF());
    Serial.print(' ');
    Serial.print(ball.getV());
    Serial.print(' ');
    Serial.println(ball.getP());
  }
  samples++;
}

void loop(){
}


