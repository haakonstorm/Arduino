
#include <Ball2.h>
#include <MsTimer2.h>
#include <EEPROM.h>
#include "pitches.h"

Ball2 ball;
int melody[] = { NOTE_F3, NOTE_B3, NOTE_DS4, NOTE_GS4,
                 NOTE_F4, NOTE_B4, NOTE_DS5, NOTE_GS5,
                 NOTE_F5, NOTE_B5, NOTE_DS6, NOTE_GS6,
                 NOTE_F6, NOTE_B6, NOTE_DS7, NOTE_GS7   };

int toneLength = 300;


void setup(){
  Serial.begin(19200);
  MsTimer2::set(5, processAD);
  MsTimer2::start();
  // Testing
  ball.setColor(0,0,0);
}

void processAD(){
  ball.processAD();
  int sensorReading = ball.getF();
  int thisPitch = map(sensorReading, 0, 300, 2000, 300);
  tone(2, thisPitch, 4);
}

int randomTone() {
  return melody[random(sizeof(melody)/sizeof(melody[0]))];
}
  

void loop(){
  if (ball.getLanded()) {
    tone(2, randomTone(), random(30,300));
    ball.setColor(0,0,255);
    ball.resetLanded();
  }
  
   if (ball.getThrown()) {
    tone(2, randomTone(), random(30,300));
    ball.setColor(0,255,0);
    ball.resetThrown();
  }
}


