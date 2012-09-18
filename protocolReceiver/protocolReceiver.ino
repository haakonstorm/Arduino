#include <Ball2.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball2 ball;
byte input_byte [5];
boolean receiving;
boolean sending;

void setup(){
  Serial.begin(19200);
  ball.setColor(0,0,0);
  sendId()
}

void loop(){
}

// Communication protocol.
// Messages are of length 5 and enclosed between byte 2 and byte 3.
// 1: ID.
//    0: all balls.
//    1-255: a particular ball.
// 2: Command.
//    C: color.
//       3-5: R,G,B
//    T: tone.
//       3-5: Higbyte for frequency, Lowbyte for frequency, Duration in 1/100 second.
//    M: message.
//       3-5: Type
//            I,I,I: Id

void runCommand() {
  // If first byte is 0, it applies to all balls. Otherwise, first byte must equal the Id.
  if (input_byte[0] == 0 || input_byte[0] == ball.getId()) {
    switch (input_byte[1]) {
      // Set color.
    case 'C': 
      {
        int rB = input_byte[2];
        int gB = input_byte[3];
        int bB = input_byte[4];
        ball.setColor(rB, gB, bB);
      }
      break;
      // Set tone.
    case 'T':
      {
        int frequency = input_byte[2] * 265 + input_byte[3];
        int duration = input_byte[4] * 10;
        tone(2, frequency, duration);
      }
      break;
    case 'M':
      {
        if (input_byte[2]  == 'I' && input_byte[3]  == 'I' && input_byte[4]  == 'I') {
          sendId();
        }
      }
      break;
    }
  }
}

void sendId() {
  sending = true;
  Serial.write(4);
  Serial.write(ball.getId());
  Serial.write(ball.getId());
  Serial.write(ball.getId());
  Serial.write(5);
  sending = false;
}

void serialEvent(){
  static byte input_position;
  static byte incoming_byte;

  while (!sending && Serial.available()) {
    incoming_byte = Serial.read();

    // If we have read five bytes.
    if (input_position == 5) {
      // We are not receiving anymore.
      receiving = false;
      // Reset counter and get ready for next message.
      input_position = 0;
      // If the last byte is 3, then it is a valid message. 
      if (incoming_byte == 3) {
        // This is the interpreter. It is only called if there is a valid message.
        runCommand();
      }
    }
    // If we are not receiving and the byte is 2, then start receiving.
    else if (receiving == false && incoming_byte == 2) {
      receiving = true;
    }
    // If we are listening, read byte (until input_position is 5).
    else if (receiving == true) {
      input_byte[input_position] = incoming_byte;
      input_position++;
    }
  }
}









