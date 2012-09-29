#include <Ball2.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball2 ball;
byte input_byte [5];
boolean receiving;
boolean sending;
byte reportType = 0;
byte reportNext = 0;
boolean reportForever = false;
boolean reportXYZFVP = false;
boolean bumpListener = false;
byte highByte, lowByte;

static int samples = 0;

void setup(){
  checkBattery();
  Serial.begin(19200);
  ball.setColor(0,0,0);
  sendInfo();
  MsTimer2::set(5, processAD);
  MsTimer2::start();
}

void loop(){
}

void processAD() {
  ball.processAD();

  if (reportXYZFVP) {
    if (samples == 50) {
      samples = 0;
      Serial.print('m');
      Serial.print(ball.getX());
      Serial.print(' ');
      Serial.print(ball.getY());
      Serial.print(' ');
      Serial.print(ball.getZ());
      Serial.print(' ');
      Serial.print(ball.getF());
      Serial.print(' ');
      Serial.print(ball.getV());
      Serial.print(' ');
      Serial.println(ball.getP());
    }
    samples++;
  }

  if (reportForever) {
    reportNext = 1;
  }

  if (reportNext > 0) {
    // Report Thrown.
    if (reportType == 1 || reportType == 7) {
      if (ball.getThrown()) {
        randomColor();
        sendThrown();
        ball.resetThrown();
      }
    } 

    // Report Landed.
    if (reportType == 2 || reportType == 7) {
      if (ball.getLanded()) {
        ball.setColor(1,1,1);
        sendLanded();
        ball.resetLanded();
      }
    }
    // Report Tapped.
    if (reportType == 3 || reportType == 7) {
      if (ball.getTapped()) {
        sendTapped();
        ball.setColor(0, 50, 0);
        ball.resetTapped();
        delay(100);
        ball.setColor(0, 0, 0);
        ball.resetTapped();
      }
    }

    // Report Force.
    if (reportType == 4) {
      sendForce();
    }

    // Report Velocity.
    if (reportType == 5) {
      sendVelocity();
    }

    // Report XYZFVP.
    if (reportType == 6) {
      reportXYZFVP = true;
    }

    reportNext--;
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
//    A: alpha / brightness.
//       3: brightness
//       4: targetBrightness
//       5: fadeSpeed
//    F: fade
//       3: (undefined)
//       4: targetBrightness
//       5: fadeSpeed
//    B: bumplistener.
//       3: 1 = start, 0 = stop
//    S: direct color.
//       3: value (0-255)
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
//          3: means Tapped
//          4: means Force
//          5: means Velocity
//          6: means XYZFVP
//          7: means Thrown, Landed, Tapped
//       4: n
//       5: 1 = report forever / 0 = stop

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

      // Set brightness.
    case 'A': 
      {
        int brightness = input_byte[2];
        int targetBrightness = input_byte[3];
        int fadeSpeed = input_byte[4];

        ball.setBrightness(brightness);
        ball.setTargetBrightness(targetBrightness);

        if (fadeSpeed != 0) {
          ball.setFadeSpeed(fadeSpeed);
        }
      }
      break;

      // Set brightness.
    case 'F': 
      {
        // int brightness = input_byte[2];
        int targetBrightness = input_byte[3];
        int fadeSpeed = input_byte[4];

        // ball.setBrightness(brightness);
        ball.setTargetBrightness(targetBrightness);

        if (fadeSpeed != 0) {
          ball.setFadeSpeed(fadeSpeed);
        }
      }
      break;
      
      // Set simple color.
    case 'S': 
      {
        int value = input_byte[2];
        ball.setSimpleColor(value);
      }
      break;
      // Set brightness.
    case 'B': 
      {
        if (input_byte[2] == 1) {
          ball.resetThrown();
          ball.resetLanded();
          ball.resetTapped();
          bumpListener = true;
          reportForever = true;
          reportType = 3;
        } 
        else {
          ball.resetThrown();
          ball.resetLanded();
          ball.resetTapped();
          bumpListener = false;
          reportForever = false;
          reportType = 0;
        }
      }
      break;      // Set tone.
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
        ball.resetTapped();
        reportType = input_byte[2];
        reportNext = input_byte[3];
        // If the last byte is 0, stop everything.
        if (input_byte[4] == 0) {
          reportForever = false;
          reportXYZFVP = false;
          reportNext = 0;
          reportType = 0;
        } 
        else if (input_byte[4] == 1) {
          reportForever = true;
          reportNext = 0;
        }
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
  Serial.write('R'); // 2. Report
  Serial.write('T'); // 3. Thrown
  Serial.write(highByte); // 4. Higbyte for number of samples
  Serial.write(lowByte); // 5. Lowbyte for number of samples
  Serial.write(5);
  sending = false;
}

void sendLanded() {
  sending = true;
  highByte = (byte) (ball.getFlyTime() / 256);
  lowByte = (byte) (ball.getFlyTime() - (highByte * 256));
  Serial.write(4);
  Serial.write(ball.getId()); // 1. ID
  Serial.write('R'); // 2. Report
  Serial.write('L'); // 3. Landed
  Serial.write(highByte); // 4. Higbyte for number of samples
  Serial.write(lowByte); // 5. Lowbyte for number of samples
  Serial.write(5);
  sending = false;
}

void sendTapped() {
  sending = true;
  Serial.write(4);
  Serial.write(ball.getId()); // 1. ID
  Serial.write('R'); // 2. Report
  Serial.write('P'); // 3. Tapped
  Serial.write((byte) 0); // 4. Unspecified.
  Serial.write((byte) 0); // 5. Unspecified.
  Serial.write(5);
  sending = false;
}

// Dummy function
void sendForce() {
  sending = true;
  Serial.write(4);
  Serial.write(ball.getId()); // 1. ID
  Serial.write('R'); // 2. Report
  Serial.write('F'); // 3. Force
  Serial.write((byte) 0); // 4. Unspecified.
  Serial.write((byte) 0); // 5. Unspecified.
  Serial.write(5);
  sending = false;
}

// Dummy function
void sendVelocity() {
  sending = true;
  Serial.write(4);
  Serial.write(ball.getId()); // 1. ID
  Serial.write('R'); // 2. Report
  Serial.write('V'); // 3. Velocity
  Serial.write((byte) 0); // 4. Unspecified.
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

void setColor(byte i){
  ball.setSimpleColor(i);
}

void randomColor() {
  ball.setSimpleColor((byte) random (256));
}

void checkBattery() {
  int flashes;
  float batteryLevel = ball.getBatteryLevel();
  if(batteryLevel>4.0f)
    flashes=6;
  else if (batteryLevel>3.95f)
    flashes=5;
  else if (batteryLevel>3.90f)
    flashes=4;
  else if (batteryLevel>3.80f)
    flashes=3;
  else if (batteryLevel>3.70f)
    flashes=2;
  else
    flashes=1;

  for(int i = 0;i<flashes;i++){
    ball.setColor(0,10,0);
    delay(50);
    ball.setColor(0,0,0);
    delay(100);
  }  
}












