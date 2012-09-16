/*
 * (c) 2012 Smarte baller med visuell forsterkning
 * E-post:   smarteballer@ifi.uio.no
 * Nettside: http://www.smarteballer.no/
 */

#include <EEPROM.h>

unsigned char id_TOEEPROM = 10;

void setup(){
  defaultColor();
  Serial.begin(19200);
  printMenu();
  showInfo();
  delay(1000);
}

void processAD() {
}

void loop(){
  if(Serial.available() > 0 && busy == false) {
    busy = true;
    radioInput = Serial.read();
    switch (radioInput) {
    case '0': 
      printMenu();
      break;
    case '1': 
      showInfo();
      break;
    case 'I': 
      writeEEPROM_ID();
      break;
    }
    while(Serial.available() > 0){
      Serial.read();
    }
    emptySerialBuffer();
    busy = false;
    MsTimer2::start();
  }
}

void printMenu(){
  defaultColor();
  Serial.println("\nSmarte baller med visuell forsterkning");
  delay(150);
  Serial.println("--------------------------------------");
  delay(150);  
  Serial.println("0. Viser denne menyen.");
  delay(150);
  Serial.println("1. Viser informasjon om denne ballen.");
  delay(150);
  Serial.println("I. Skriver id til EEPROM.");
  delay(150);
}

void showInfo(){
  Serial.println("\nInformasjon om tilkoblet ball:");
  delay(150);
  delay(150);
  Serial.println("--------------------------------------");  
  delay(150);
  Serial.print("Ball-nummer:            ");
  delay(150);
  Serial.println(String(ball.getId(), DEC));
  delay(150);
  Serial.print("Batterispenning:        ");
  delay(150);
  Serial.println(ball.getBatteryLevel());
  delay(150);
  Serial.print("x-verdi ved fritt fall: ");
  delay(150);
  Serial.println((EEPROM.read(xEepromHigh) * 256) + EEPROM.read(xEepromLow));
  delay(150);
  Serial.print("y-verdi ved fritt fall: ");
  delay(150);
  Serial.println((EEPROM.read(yEepromHigh) * 256) + EEPROM.read(yEepromLow));
  delay(150);
  Serial.print("z-verdi ved fritt fall: ");
  delay(150);
  Serial.println((EEPROM.read(zEepromHigh) * 256) + EEPROM.read(zEepromLow));
  delay(150);
}

void writeEEPROM_ID(){

  ball.setColor(0,0,255);

  Serial.println("\nSKRIVER TIL EEPROM!");
  delay(150);
  Serial.println("--------------------------------------");
  delay(150);

  Serial.print("Ball-nummer:            ");
  delay(150);
  Serial.println(String(id_TOEEPROM, DEC));
  delay(150);
  EEPROM.write(ID,id_TOEEPROM);
  delay(150);
  Serial.print("SJEKKER... ");
  delay(150);
  if(id_TOEEPROM == EEPROM.read(ID)) {
    Serial.println("OK!");
    delay(150);
  } 
  else {
    Serial.println("FEIL!");
    delay(150);
  }
  ball.setColor(0,255,0);
  showInfo(); 
}


void defaultColor() {
  ball.setColor(1,1,1);
}

void emptySerialBuffer() {
  while(Serial. available())
    Serial.read();
}










