#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;

// States

#define PASSIVE 0
#define FORCE_GAME 1
#define SITESWAP 2

// receiver
byte firstByte;

// colors
byte rB = 0;
byte gB = 0;
byte bB = 0;

// predictions
static int sampleCounter = 0;
static unsigned char prevSiteSwaps = 1;
static int colorCounter = 0;
unsigned char historyP[10];
unsigned char historyPP[10][10];
static unsigned char currentSiteSwap = 1;
static unsigned char previousSiteSwap = 0;
static unsigned char previousPreviousSiteSwap = 0;
static unsigned char predictedSiteSwap = 0;
static unsigned char predictionConfidence = 0;

// force game
unsigned char balls [4] = {0,5,6,7};
static int forceCounter = 0;
static boolean locked;
static unsigned char lock;
byte radioInput;
unsigned char randomBall;
byte randomByte;

// state
static byte state = 1;
static byte newState;

void setup(){
  Serial.begin(19200);
  MsTimer2::set(5, processAD);
  MsTimer2::start();
  ball.setColor(1,1,1);
}

void processAD(){
  ball.processAD();
}

void loop(){
  if (Serial.peek() == '.') {
    Serial.flush();
    state = PASSIVE;
    color(0,0,0);
  }
  
  if (Serial.peek() == ':') {
    color(0,0,255);
    radioInput = Serial.read();
    state = Serial.read();
    Serial.flush();
  }
  
  if (state == FORCE_GAME) {
    setColorSS(4);
    if (locked && (lock == 0)) {
      forceCounter = forceCounter + max(0,ball.getF()-120);
      if (forceCounter > 1000) {
        locked = false;
        forceCounter = 0;
        color(0,0,1);
        callNext();
      }
    }
    else if (locked && (lock == 1)) {
      if (ball.getInAir()) {
        locked = false;
        color(0,0,1);
        callNext();
      }
    }
    else if (!locked && (Serial.available() > 0 )) {
      radioInput = Serial.read();
      if (radioInput == ball.getId()) {
        activateLock();
      }
    }
  }
//
//  if (state == REMOTE_CONTROL) {
//    if(Serial.available() >= 4){
//      firstByte = Serial.read();
//      if (firstByte = 'c') {
//        int rB = Serial.read();
//        int gB = Serial.read();
//        int bB = Serial.read();
//        ball.setColor(rB, gB, bB);
//        Serial.flush();
//      }
//    }
//  }

  if (state == SITESWAP) {
    setColorSS(5);
    if (Serial.available()) {
      Serial.flush();
      currentSiteSwap++;
    }

    if(ball.getLanded()){
      ball.resetLanded();
      setColorSS(0);
    }

    if(ball.getThrown()){

      // update of prediction arrays
      if (predictionConfidence == 2) { // a prediction with confidence 2 was made for this throw
        if (predictedSiteSwap != currentSiteSwap) { // a wrong prediction was made
          historyPP[previousPreviousSiteSwap][previousSiteSwap] = 0;  // reset prediction value
        }
      }
      else if (predictionConfidence == 1) { // a prediction with confidence 1 was made for this throw
        if (predictedSiteSwap != currentSiteSwap) { // a wrong prediction was made
          historyP[previousSiteSwap] = 0; // reset prediction value
        }
      }

      historyP[previousSiteSwap] = currentSiteSwap;
      historyPP[previousPreviousSiteSwap][previousSiteSwap] = currentSiteSwap;

      // move history
      previousPreviousSiteSwap = previousSiteSwap;
      previousSiteSwap = currentSiteSwap;
      currentSiteSwap = 1;

      // make prediction
      if (historyPP[previousPreviousSiteSwap][previousSiteSwap] > 0) {
        predictedSiteSwap = historyPP[previousPreviousSiteSwap][previousSiteSwap];
        predictionConfidence = 2;
      } 
      else if (historyP[previousSiteSwap] > 0) {
        predictedSiteSwap = historyP[previousSiteSwap];
        predictionConfidence = 1;
      } 
      else {
        predictedSiteSwap = previousSiteSwap;
        predictionConfidence = 0;
      }

      setColorSS(predictedSiteSwap);

      ball.resetThrown();
      Serial.print((int) predictedSiteSwap);
    }
  }

}

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

void setColorSS(int i) {
  switch (i) {
  case 0:
    ball.setColor(1,1,1); // DIM
    break;
  case 1:
    ball.setColor(255,0,0); // RED
    break;
  case 2:
    ball.setColor(255,255,0); // YELLOW
    break;
  case 3:
    ball.setColor(0,255,0); // GREEN
    break;
  case 4:
    ball.setColor(0,0,255); // BLUE
    break;
  case 5:
    ball.setColor(128,0,255); // VIOLET
    break;
  case 6:
    ball.setColor(255,255,255); // FUCHSIA
    break;
  default:
    ball.setColor(1,1,1); // DIM
    break;
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

void callNext() {
  randomBall = (byte) balls[random(4)];
  if (randomBall == ball.getId()) {
    callNext();
  } 
  else {
    Serial.write(randomBall);
  }
}







