#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;

void setup(){
  Serial.begin(19200);
  MsTimer2::set(5, processAD);
  MsTimer2::start();
  ball.setColor(0,0,0);
  showInfo();
  delay(1000);
}


void processAD() {
  static int samples = 0;
  static int force;
  ball.processAD();
  if(samples >= 100) {
    samples = 0;
    force = ball.getF();
    Serial.println(force);
  }
  samples++;
}

void loop(){
  static byte firstByte;
  static int r = 0;
  static int g = 0;
  static int b = 0;
  
  if(Serial.available()){
    firstByte = Serial.read();
    switch (firstByte) {
      case 'R': r = 255; break;
      case 'G': g = 255; break;
      case 'B': b = 255; break;
      case 'r': r = 0; break;
      case 'g': g = 0; break;
      case 'b': b = 0; break;
    }
    ball.setColor(r, g, b);
  }
}


void showInfo(){
  Serial.println("\nInformasjon om tilkoblet ball:");
  Serial.println("--------------------------------------");  
  Serial.print("Ball-nummer:            ");
  Serial.println(ball.getId());
  Serial.print("Batterispenning:        ");
  Serial.println(ball.getBatteryLevel());
  Serial.print("x-verdi ved fritt fall: ");
  Serial.println((EEPROM.read(xEepromHigh) * 256) + EEPROM.read(xEepromLow));
  Serial.print("y-verdi ved fritt fall: ");
  Serial.println((EEPROM.read(yEepromHigh) * 256) + EEPROM.read(yEepromLow));
  Serial.print("z-verdi ved fritt fall: ");
  Serial.println((EEPROM.read(zEepromHigh) * 256) + EEPROM.read(zEepromLow));
}

void readEEPROM(){
  Serial.println("\nHele innholdet av EEPROM:");
  Serial.println("--------------------------------------");
  byte value;
  for (int address=0; address<512; address++){
    value = EEPROM.read(address);
    Serial.print(address);
    Serial.print("\t");
    Serial.print(value, DEC);
    Serial.println();
  }
}



