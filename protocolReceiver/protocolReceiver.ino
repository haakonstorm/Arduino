#include <Ball2.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball2 ball;
byte input_byte [5];
boolean receiving;
boolean sending;
byte reportType = 0;
byte reportNext = 0;
byte highByte, lowByte;

void setup(){
  Serial.begin(19200);
  sendInfo();
  ball.setColor(0,0,0);
  MsTimer2::set(5, processAD);
  MsTimer2::start();
}

void loop(){
}

void processAD() {
  ball.processAD();

  if (reportNext > 0) {
    if (reportType == 1) {
      if (ball.getThrown()) {
        sendThrown();
        reportNext--;
        ball.resetThrown();
      }
    }
    else if (reportType == 2) {
      if (ball.getLanded()) {
        sendLanded();
        reportNext--;
        ball.resetLanded();
      }
    }
  }
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
//    I: info.
//       3-5: 0,0,0: Id
//    E: Write to EEPROM.
//       3 and 5: Type
//          6: ID
//          7: SPEAKER
//       4: Value
//    R: Report next n instances of type.
//       3: Type
//          1: means Thrown
//          2: means Landed
//       4: n
//       5: unspecified

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
        int frequency = input_byte[2] * 256 + input_byte[3];
        int duration = input_byte[4] * 10;
        tone(2, frequency, duration);
      }
      break;
      // Send info.
    case 'I':
      {
        // If all values are 0, send info.
        if (input_byte[2]  == 0 && input_byte[3]  == 0 && input_byte[4]  == 0) {
          sendInfo();
        }
      }
      break;
    case 'E':
      {
        // If value 3 and 5 are 6, then write ID to EEPROM.
        if (input_byte[2]  == 6 && input_byte[4]  == 6) {
          EEPROM.write(ID,input_byte[3]);
          if(input_byte[3] == EEPROM.read(ID)) {
            reportOK();
          } 
          else {
            reportNotOK();
          }
          sendInfo();
        } 
        else 
          // If value 3 and 5 are 6, then write SPEAKER to EEPROM.
        if (input_byte[2]  == 7 && input_byte[4]  == 7) {
          EEPROM.write(SPEAKER,input_byte[3]);
          if(input_byte[3] == EEPROM.read(SPEAKER)) {
            reportOK();
          } 
          else {
            reportNotOK();
          }
          sendInfo();
        }
      }
      break;
      // Set report type and number
    case 'R': 
      {
        ball.resetThrown();
        ball.resetLanded();
        reportType = input_byte[2];
        reportNext = input_byte[3];
      }
      break;
    }
  }
}

void sendInfo() {
  sending = true;
  Serial.write(4);
  Serial.write(ball.getId());  
  Serial.write('I');
  Serial.write(ball.hasSpeaker());
  Serial.write((byte) ((ball.getBatteryLevel() - 2.0) * 100));
  Serial.write(ball.getInAir());
  Serial.write(5);
  sending = false;
}

void sendThrown() {
  sending = true;
  highByte = (byte) (ball.getHoldTime() / 256);
  lowByte = (byte) (ball.getHoldTime() - (highByte * 256));
  Serial.write(4);
  Serial.write(ball.getId()); // 1. ID
  Serial.write('T'); // 2. Thrown
  Serial.write(highByte); // 3. Higbyte for number of samples
  Serial.write(lowByte); // 4. Lowbyte for number of samples
  Serial.write((byte) 0); // 5. Unspecified.
  Serial.write(5);
  sending = false;
}

void sendLanded() {
  sending = true;
  highByte = (byte) (ball.getFlyTime() / 256);
  lowByte = (byte) (ball.getFlyTime() - (highByte * 256));
  Serial.write(4);
  Serial.write(ball.getId()); // 1. ID
  Serial.write('L'); // 2. Landed
  Serial.write(highByte); // 3. Higbyte for number of samples
  Serial.write(lowByte); // 4. Lowbyte for number of samples
  Serial.write((byte) 0); // 5. Unspecified.
  Serial.write(5);
  sending = false;
}

void serialEvent(){
  static byte input_position;
  static byte incoming_byte;

  while (!sending && Serial.available()) {
    // Read one byte.
    incoming_byte = Serial.read();
    // If we have read five bytes.
    if (receiving && input_position == 5) {
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
    else if (!receiving && incoming_byte == 2) {
      receiving = true;
    }
    // If we are listening, read byte (until input_position is 5).
    else if (receiving) {
      input_byte[input_position] = incoming_byte;
      input_position++;
    }
  }
}

void reportOK() {
  ball.setColor(0,255,0);
  delay(200);
  ball.setColor(0,0,0);
  delay(200);
  ball.setColor(0,255,0);
  delay(200);
  ball.setColor(0,0,0);
  delay(200);
  ball.setColor(0,255,0);
  delay(200);
  ball.setColor(0,0,0);
}

void reportNotOK() {
  ball.setColor(255,0,0);
  delay(200);
  ball.setColor(0,0,0);
  delay(200);
  ball.setColor(255,0,0);
  delay(200);
  ball.setColor(0,0,0);
  delay(200);
  ball.setColor(255,0,0);
  delay(200);
  ball.setColor(0,0,0);
}






