
#include <Ball2.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball2 ball;

byte idByte = (byte) ball.getId() << 5; 
byte radioByte;
byte radioByteID;
byte radioByteSS;

// radioByteSS
//0 0
//1 0
//2 0
//3 1 lav
//4 1 sikker
//5 1 høy
//6 2 lav
//7 2 sikker
//8 2 høy
//9 3 lav
//10 3 sikker
//11 3 høy
//12 4 lav
//13 4 sikker



byte currentSiteSwap = 1;
byte predictedSiteSwap = 0;

byte state[11];

int forceThreshold[32];

void setup(){
  Serial.begin(19200);
  MsTimer2::set(5, processAD);
  MsTimer2::start();
  ball.setSiteSwapColor(0);
  forceThreshold[6] = 3000;
  forceThreshold[7] = 4000;
  forceThreshold[8] = 5000;
  forceThreshold[9] = 6000;
  forceThreshold[10] = 7000; // alt over er sikker 3
  forceThreshold[11] = 8000;
  forceThreshold[12] = 9000;
  forceThreshold[13] = 10000; // sikker 4
}

void processAD(){

  ball.processAD();

  if(ball.getLanded()){
    ball.resetLanded();
    ball.setSiteSwapColor(0);
  }

  if(ball.getThrown()){
    ball.resetThrown();
    if (state[1] == 0) {
      radioByteSS = 4;
    } else {
      radioByteSS = predictSiteSwapFromForce(ball.getForceIntegral());
    }
    
    radioByte = radioByteSS | idByte;
    
    predictedSiteSwap = radioByteSS / 3;
    state[predictedSiteSwap] = radioByte;
    shiftStates();
    
    ball.setSiteSwapColor(predictedSiteSwap);
    Serial.write(radioByte);
  }
}

void serialEvent() {
  radioByte = Serial.read();
  // radioByteID = (radioByte >> 5);
  radioByteSS = radioByte & B00011111;
  
  predictedSiteSwap = radioByteSS / 3;
  state[predictedSiteSwap] = radioByte;
  shiftStates();
}





void shiftStates() {
  for (int i = 0; i < 9; i++) {
    state[i] = state[i+1];
  }
  state[9] = 0;
}

inline byte predictSiteSwapFromForce(int force) {
  for (int i = 6; i < 32; i++) {
    if (force < forceThreshold[i]) {
          return i;
    }
  }
  
  return 0;
}


void loop() {
}


