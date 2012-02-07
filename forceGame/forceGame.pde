#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;
static byte id = 1; //((byte) ball.getId());
static byte numberOfBalls = 4;
static int forceCounter = 0;
static unsigned char lock = 0;
byte radioInput;
byte randomByte;
int x,y,z,force = 0;
int previousForce = 0;
int limitA = 250;
int limitB = 350;

void setup(){
  Serial.begin(19200);
  MsTimer2::set(5, processAD);
  MsTimer2::start();
  activateLock();
  //callNext();
}

void processAD() {
  ball.processAD();
  
  x = analogRead(X) - 512;
  y = analogRead(Y) - 512;
  z = analogRead(Z) - 512;
  previousForce = force;
  force = sqrt(x*x + y*y + z*z);//abs(x) + abs(y) + abs(z);
    
  if (lock == 1) {
    if (force > limitA && previousForce > limitA) {
      lock = 0;
      ball.setColor(0,0,0);
      callNext();
    }
  }
  else if (lock == 2) {
    if (force > limitB && previousForce > limitB) {
      lock = 0;
      ball.setColor(0,0,0);
      callNext();
    }
  }

  if(ball.getInAir()) {
    ball.setColor(2,2,2);
  }
  else{
    ball.setColor(0,0,0);
  }
}

void loop() {
  if(Serial.available() > 0 ) {
    radioInput = Serial.read();
    if (radioInput == id) {
      activateLock();
    }
  }
}

void activateLock() {
  Serial.println("activateLock");
  randomByte = (byte) random(2);
  switch (randomByte) {
  case 0:
    Serial.println("Lock 1");
    lock = 1;
    ball.setColor(255,0,0);
    break;
  case 1:
    Serial.println("Lock 2");
    lock = 2;
    ball.setColor(0,255,0);
    break;
  }
}

byte nextId(byte x) {
  if (x == (numberOfBalls - 1)) {
    return 0;
  } 
  else {
    return (x + 1);
  }
}

void callNext() {
  Serial.println("callNext");
  randomByte = (byte) random(numberOfBalls);
  if (randomByte == id) {
    callNext();
  } 
  else {
    Serial.write(randomByte);
  }
}












