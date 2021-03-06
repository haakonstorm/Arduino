#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;

static unsigned char minX = 1023;
static unsigned char minY = 1023;
static unsigned char minZ = 1023;
static unsigned char maxX = 0;
static unsigned char maxY = 0;
static unsigned char maxX = 0;

const byte expcolor[] = {
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

void setup(){
  Serial.begin(19200);
  MsTimer2::set(5, processAD);
  MsTimer2::start();
  setColorSS(0);
}

void processAD(){
  ball.processAD();
  minX = min(minX, ball.getX());
  minY = min(minY, ball.getY());
  minZ = min(minZ, ball.getZ());
  maxX = max(maxX, ball.getX());
  maxY = max(maxY, ball.getY());
  maxZ = max(maxZ, ball.getZ());
}

void sendColor(){
  static byte val_R = 0;
  static byte val_G = 0;
  static byte val_B = 0;
  
  val_R = expcolor[map(min(maxX,max(minX,ball.getX())),minX,maxX,1,99)];
  val_G = expcolor[map(min(maxY,max(minY,ball.getY())),minY,maxY,1,99)];
  val_B = expcolor[map(min(maxZ,max(minZ,ball.getZ())),minZ,maxZ,1,99)];

  ball.setColor(val_R, val_G, val_B);
}

void loop() {
}


