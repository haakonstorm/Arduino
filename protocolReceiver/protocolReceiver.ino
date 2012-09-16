#include <Ball2.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball2 ball;
byte input_byte [5];

void setup(){
  Serial.begin(19200);
  ball.setColor(0,0,0);
}

void loop(){
  sendSound(
}

void runCommand() {
  if (input_byte[0] == 0 || input_byte[0] == ball.getId()) {
    switch (input_byte[1]) {
    case 'C':
      int rB = input_byte[2];
      int gB = input_byte[3];
      int bB = input_byte[4];
      ball.setColor(rB, gB, bB);
      break;
    case 'L':
      int frequency = input_byte[2] * 265 + input_byte[3];
      int duration = input_byte[4] * 10;
      tone(2, frequency, duration);
      break;
    }
  }
}

void serialEvent(){
  static boolean active;
  static byte input_position;
  static byte incoming_byte;
  while (Serial.available()) {
    incoming_byte = Serial.read();
    if (input_position == 5) {
      active = false;
      input_position = 0;
      if (incoming_byte == 3) {
        runCommand();
      }
    } 
    else if (active == false && incoming_byte == 2) {
      active = true;
    } 
    else if (active == true) {
      input_byte[input_position] = incoming_byte;
      input_position++;
    }
  }
}




