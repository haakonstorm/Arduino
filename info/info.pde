/*
 * (c) 2011 Smarte baller med visuell forsterkning
 * E-post:   smarteballer@ifi.uio.no
 * Nettside: http://www.smarteballer.no/
 */

#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;
byte radioInput;

void setup(){
  Serial.begin(19200);
  printMenu();
  showInfo();
}

void loop(){
  if(Serial.available() > 0 ) {
    radioInput = Serial.read();
    Serial.flush();
    switch (radioInput) {
    case '0': 
      printMenu();
      break;
    case '1': 
      showInfo();
      break;
    case '2': 
      readEEPROM();
      break;
    }
  }
}

void printMenu(){
  Serial.println("\nSmarte baller med visuell forsterkning");
  Serial.println("--------------------------------------");  
  Serial.println("0. Vis meny.");
  Serial.println("1. Vis informasjon om denne ballen.");  
  Serial.println("2. Skriv ut hele EEPROM."); 
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





