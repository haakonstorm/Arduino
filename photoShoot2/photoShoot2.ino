#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;
int R[256] = {
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,251,245,240,234,228,222,215,209,203,197,192,186,179,173,167,161,155,149,144,138,132,126,120,113,107,101,96,89,83,77,71,65,59,53,48,42,36,30,24,17,11,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,11,17,23,29,35,41,47,53,60,66,72,78,84,90,96,101,107,113,119,125,131,137,143,149,156,162,168,174,180,186,192,197,203,209,215,221,227,233,239,245,252,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};
int G[256] = {
  0,6,12,18,24,30,36,42,48,54,60,65,72,78,84,89,96,102,108,113,120,126,131,138,144,150,156,162,168,174,179,186,192,198,204,210,216,222,227,234,240,246,252,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,251,245,240,234,227,221,215,209,203,197,192,186,179,173,167,161,155,149,144,138,131,125,119,113,107,101,96,90,83,77,71,65,59,53,48,42,35,29,23,17,11,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int B[256] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,12,18,24,30,36,42,48,54,60,66,72,78,84,90,96,102,108,114,120,126,132,138,144,150,156,162,168,174,180,186,192,198,204,210,216,222,228,234,240,246,252,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,251,245,240,234,228,222,216,209,203,197,191,185,179,173,167,161,155,149,144,138,132,126,120,113,107,101,95,89,83,77,71,65,59,53,48,42,36,30,24,17,11,5,0};

boolean blinkReady = true;

void setup() {
  Serial.begin(19200);
  MsTimer2::set(1, processAD);
  MsTimer2::start();
  // ball.setColor(1,1,1);
}

static int samples = 0;

void processAD() {
  ball.processAD();

  if (ball.getThrown()) {
    setColor(0);
    ball.resetThrown();
  }

  if (ball.getLanded()) {
    setColor(128);
    ball.resetLanded();
  }
  
  if (ball.getInAir() && ball.getV() == 0) {
    setColor(88);
  } else {
    setColor(0
    );
  }
}

void loop(){
}

void setColor(byte i){
  ball.setColor(R[i], G[i], B[i]);
}

void randomColor() {
  setColor((int) random (256));
}


