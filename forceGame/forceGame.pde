#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

int x, y, z;

Ball ball;

static byte id = 3;
static byte maxId = 3;
static int forceCounter = 0;
static boolean locked;
static unsigned char lock;
byte radioInput;
byte randomByte;

void setup(){
  Serial.begin(19200);
  MsTimer2::set(5, processAD);
  MsTimer2::start();
  callNext();
}

void processAD() {
  ball.processAD();
  x = analogRead(X);
  y = analogRead(Y);
  z = analogRead(Z);

  if (locked && (lock == 0)) {
    forceCounter = forceCounter + max(0,max(max(x,y),z)-600);
    if (forceCounter > 5000) {
      locked = false;
      forceCounter = 0;
      color(0,0,1);
      callNext();
    }
  }
  else if (locked && (lock == 1)) {
    if (((abs(500-x) < 40) && (abs(500-y) < 40) && (abs(500-z) < 40))) {
      locked = false;
      color(0,0,1);
      callNext();
    }
  }
}

//  if(ball.getInAir()) {
//    color(255,0,0);
//  }
//  else{
//    color(0,255,0);
//  }

void color(unsigned char R, unsigned char G, unsigned char B) {
  // table of exponential values
  // generated for values of i from 0 to 255
  // for i=0:255 x(i+1) = round(power(2,i/32)-1); end

  static const byte expcolor [256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 
    3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 
    5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7,
    7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 10, 10,
    10, 11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15,
    15, 15, 16, 16, 16, 17, 17, 18, 18, 18, 19, 19, 20, 20, 21, 21, 
    22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 28, 28, 29, 30, 30,
    31, 32, 32, 33, 34, 35, 35, 36, 37, 38, 39, 40, 40, 41, 42, 43,
    44, 45, 46, 47, 48, 49, 51, 52, 53, 54, 55, 56, 58, 59, 60, 62,
    63, 64, 66, 67, 69, 70, 72, 73, 75, 77, 78, 80, 82, 84, 86, 88, 
    90, 91, 94, 96, 98, 100, 102, 104, 107, 109, 111, 114, 116, 119,
    122, 124, 127, 130, 133, 136, 139, 142, 145, 148, 151, 155, 158,
    161, 165, 169, 172, 176, 180, 184, 188, 192, 196, 201, 205, 210,
    214, 219, 224, 229, 234, 239, 244, 250
  };

  ball.setColor(expcolor[R],expcolor[G],expcolor[B]);
}

void loop() {
  if(Serial.available() > 0 ) {
    //    color(255,255,255);
    //    delay(10);
    //    color(1,1,1);
    radioInput = Serial.read();
    if (radioInput == id) {
      activateLock();
    }
  }
}

void activateLock() {
  randomByte = (byte) random(2);
  switch (randomByte) {
  case 0: 
    lock = 0;
    color(0,255,0);
    break;
  case 1: 
    lock = 1;
    color(255,0,0);
    break;
  }
  locked = true;
}

byte nextId(byte x) {
  if (x == maxId) {
    return 0;
  } 
  else {
    return (x + 1);
  }
}

void callNext() {
  randomByte = (byte) random(maxId+1);
  if (randomByte == id) {
    callNext();
  } 
  else {
    Serial.write(randomByte);
  }
}











