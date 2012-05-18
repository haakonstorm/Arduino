/*
 * (c) 2011 Smarte baller med visuell forsterkning
 * E-post:   smarteballer@ifi.uio.no
 * Nettside: http://www.smarteballer.no/
 */

#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;
unsigned char radioInput;

unsigned char id_TOEEPROM = 4;
int x_TOEEPROM = 503;
int y_TOEEPROM = 500;
int z_TOEEPROM = 511;

unsigned char Xcount[60];
unsigned char Ycount[60];
unsigned char Zcount[60];
unsigned char Xmax, Ymax, Zmax;

boolean showXYZ = false;
boolean filtered = true;
boolean showForce = false;
boolean colorInAir = false;
boolean busy = false;

void setup(){
  defaultColor();
  initCounters();
  Serial.begin(19200);
  printMenu();
  showInfo();
  delay(1000);
  MsTimer2::set(5, processAD);
  MsTimer2::start();
}

static int samples = 0, averageSamples = 0;
static int x, y, z, force;
static float xA = 0, yA = 0, zA = 0;
static byte rB, gB, bB;

void processAD() {
  samples++;
  if (!busy) {
    busy = true;
    ball.processAD();

    if(showXYZ & (samples%10==0)){
      x = analogRead(X);
      y = analogRead(Y);
      z = analogRead(Z);
      if (!filtered || ((abs(500-x) < 30) && (abs(500-y) < 30) && (abs(500-z) < 30))) {
        xA = (x + (averageSamples * xA)) / (averageSamples + 1);
        yA = (y + (averageSamples * yA)) / (averageSamples + 1);
        zA = (z + (averageSamples * zA)) / (averageSamples + 1);
        averageSamples++;
        Xcount[530-x]++;
        Ycount[530-y]++;
        Zcount[530-z]++;
        setMax();

        if (samples%30==0){
          
          ball.setColor(255,0,0);
          
          // X, Y, and Z
          Serial.print(" XYZ: ");
          Serial.print(x);
          Serial.print(" ");
          Serial.print(y);
          Serial.print(" ");
          Serial.print(z);

          // MAX-COUNTER
          Serial.print(" \tMAX: ");
          Serial.print(530-Xmax);
          Serial.print(" (");
          Serial.print(String(Xcount[Xmax], DEC));
          Serial.print(") ");
          Serial.print(530-Ymax);
          Serial.print(" (");
          Serial.print(String(Ycount[Ymax], DEC));
          Serial.print(") ");
          Serial.print(530-Zmax);
          Serial.print(" (");
          Serial.print(String(Zcount[Zmax], DEC));
          Serial.print(") ");

          // RUNNING AVERAGE
          Serial.print(" \tAVG: ");
          Serial.print(xA);
          Serial.print(" ");
          Serial.print(yA);
          Serial.print(" ");
          Serial.print(zA);

          Serial.print("\n");
          
          ball.setColor(0,0,0);
          
        }
      }
    }

    if(showForce & (samples%25==0)){
      force = ball.getF();
      Serial.print ("Kraften som virker paa ballen: ");
      delay(100);
      Serial.println (force);
      delay(100);
    }

    if(colorInAir){
      if(ball.getInAir()) {
        ball.setColor(255,0,0);
      }
      else{
        ball.setColor(0,255,0);
      }
    }
  }
  busy = false;
}


void loop(){
  if(Serial.available() > 0) {
    busy = true;
    radioInput = Serial.read();
    switch (radioInput) {
    case 'C':
      if(Serial.available() >= 3) {
        rB = Serial.read();
        gB = Serial.read();
        bB = Serial.read();
        ball.setColor(rB, gB, bB);
      }
      break;
    case '0': 
      printMenu();
      break;
    case '1': 
      showInfo();
      break;
    case '2': 
      readEEPROM();
      break;
    case 'm': 
      showXYZ = !showXYZ;
      if (showXYZ) {
        Serial.println("\nStarter maaling av x-, y- og z-verdier.");
        delay(100);
        Serial.println("--------------------------------------");
        delay(100);
      } 
      else {
        Serial.println("\nStopper maaling av x-, y- og z-verdier.");
        delay(100);
        Serial.println("--------------------------------------");
        delay(100);
      }
      break;
    case 'n':
      filtered = !filtered;
      if (filtered) {
        Serial.println("\nSlaar paa filtrering av x-, y- og z-verdier.");
        delay(100);
        Serial.println("--------------------------------------");
        delay(100);
      } 
      else {
        Serial.println("\nSlaar av filtrering av x-, y- og z-verdier.");
        delay(100);
        Serial.println("--------------------------------------");
        delay(100);
      }
      break; 
    case 'f':
      showForce = !showForce;
      if (showForce) {
        Serial.println("\nStarter maaling av kraften som virker paa ballen.");
        delay(100);
        Serial.println("--------------------------------------");
        delay(100);
      } 
      else {
        Serial.println("\nStopper maaling av kraften som virker paa ballen.");
        delay(100);
        Serial.println("--------------------------------------");
        delay(100);
      }
      break;
    case 'i':
      colorInAir = !colorInAir;
      if (colorInAir) {
        Serial.println("\nStarter programmet som bytter farge naar ballen er i luften.");
        delay(100);
        Serial.println("--------------------------------------");
        delay(100);
      } 
      else {
        Serial.println("\nStopper programmet som bytter farge naar ballen er i luften.");
        delay(100);
        Serial.println("--------------------------------------");
        delay(100);
      }
      break;
    case 'E': 
      writeEEPROM();
      break;
    case 'I': 
      writeEEPROM_ID();
      break;
    }
    while(Serial.available() > 0){
      // ball.setColor(0,0,255);
      Serial.read();
    }
  }
  busy = false;
}

void printMenu(){
  defaultColor();
  Serial.println("\nSmarte baller med visuell forsterkning");
  delay(100);
  Serial.println("--------------------------------------");
  delay(100);  
  Serial.println("0. Viser denne menyen.");
  delay(100);
  Serial.println("1. Viser informasjon om denne ballen.");
  delay(100);
  Serial.println("2. Viser alt innhold i EEPROM.");
  delay(100);
  Serial.println("f. Starter eller stopper maaling av kraften som virker paa ballen.");
  delay(100);
  Serial.println("m. Starter eller stopper maaling av x-, y- og z-verdier.");
  delay(100);
  Serial.println("n. Starter eller stopper filtrering av x-, y- og z-verdier.");
  delay(100);
  Serial.println("i. Starter eller stopper programmet som bytter farge naar ballen er i luften.");
  delay(100);
  Serial.println("E. Skriver id, x, y og z til EEPROM.");
  delay(100);
  Serial.println("I. Skriver id til EEPROM.");
  delay(100);
}

void showInfo(){
  Serial.println("\nInformasjon om tilkoblet ball:");
  delay(100);
  delay(100);
  Serial.println("--------------------------------------");  
  delay(100);
  Serial.print("Ball-nummer:            ");
  delay(100);
  Serial.println(String(ball.getId(), DEC));
  delay(100);
  Serial.print("Batterispenning:        ");
  delay(100);
  Serial.println(ball.getBatteryLevel());
  delay(100);
  Serial.print("x-verdi ved fritt fall: ");
  delay(100);
  Serial.println((EEPROM.read(xEepromHigh) * 256) + EEPROM.read(xEepromLow));
  delay(100);
  Serial.print("y-verdi ved fritt fall: ");
  delay(100);
  Serial.println((EEPROM.read(yEepromHigh) * 256) + EEPROM.read(yEepromLow));
  delay(100);
  Serial.print("z-verdi ved fritt fall: ");
  delay(100);
  Serial.println((EEPROM.read(zEepromHigh) * 256) + EEPROM.read(zEepromLow));
  delay(100);
}

void readEEPROM(){
  Serial.println("\nHele innholdet av EEPROM:");
  delay(100);
  Serial.println("--------------------------------------");
  delay(100);
  byte value;
  for (int address=0; address<512; address++){
    value = EEPROM.read(address);
    Serial.print(address);
    delay(100);
    Serial.print("\t");
    delay(100);
    Serial.print(value, DEC);
    delay(100);
    Serial.println();
    delay(100);
  }
}

void writeEEPROM(){
  int xH, xL;
  int yH, yL;
  int zH, zL;

  xH = (int) x_TOEEPROM / 256;
  xL = x_TOEEPROM - (xH * 256);

  yH = (int) y_TOEEPROM / 256;
  yL = y_TOEEPROM - (yH * 256);

  zH = (int) z_TOEEPROM / 256;
  zL = z_TOEEPROM - (zH * 256);

  ball.setColor(255,0,0);

  Serial.println("\nSKRIVER TIL EEPROM!");
  delay(100);
  Serial.println("--------------------------------------");
  delay(100);

  Serial.print("Ball-nummer:            ");
  delay(100);
  Serial.println(String(id_TOEEPROM, DEC));
  delay(100);
  EEPROM.write(ID,id_TOEEPROM);
  delay(100);
  Serial.print("SJEKKER... ");
  delay(100);
  if(id_TOEEPROM == EEPROM.read(ID)) {
    Serial.println("OK!");
    delay(100);
  } 
  else {
    Serial.println("FEIL!");
    delay(100);
  }

  Serial.print("x-verdi ved fritt fall: ");
  delay(100);
  Serial.println(x_TOEEPROM);
  delay(100);
  EEPROM.write(xEepromHigh,xH);
  delay(100);
  EEPROM.write(xEepromLow,xL);
  delay(100);
  Serial.print("SJEKKER... ");
  delay(100);
  if(x_TOEEPROM == ((EEPROM.read(xEepromHigh) * 256) + EEPROM.read(xEepromLow))) {
    Serial.println("OK!");
    delay(100);
  } 
  else {
    Serial.println("FEIL!");
    delay(100);
  }

  Serial.print("y-verdi ved fritt fall: ");
  delay(100);
  Serial.println(y_TOEEPROM);
  delay(100);
  EEPROM.write(yEepromHigh,yH);
  delay(100);
  EEPROM.write(yEepromLow,yL);
  delay(100);
  Serial.print("SJEKKER... ");
  delay(100);
  if(y_TOEEPROM == ((EEPROM.read(yEepromHigh) * 256) + EEPROM.read(yEepromLow))) {
    Serial.println("OK!");
    delay(100);
  } 
  else {
    Serial.println("FEIL!");
    delay(100);
  }

  Serial.print("z-verdi ved fritt fall: ");
  delay(100);
  Serial.println(z_TOEEPROM);
  delay(100);
  EEPROM.write(zEepromHigh,zH);
  delay(100);
  EEPROM.write(zEepromLow,zL);
  delay(100);
  Serial.print("SJEKKER... ");
  if(z_TOEEPROM == ((EEPROM.read(zEepromHigh) * 256) + EEPROM.read(zEepromLow))) {
    Serial.println("OK!");
    delay(100);
  } 
  else {
    Serial.println("FEIL!");
    delay(100);
  }

  ball.setColor(0,255,0);

  showInfo(); 
}

void writeEEPROM_ID(){
  int xH, xL;
  int yH, yL;
  int zH, zL;

  xH = (int) x_TOEEPROM / 256;
  xL = x_TOEEPROM - (xH * 256);

  yH = (int) y_TOEEPROM / 256;
  yL = y_TOEEPROM - (yH * 256);

  zH = (int) z_TOEEPROM / 256;
  zL = z_TOEEPROM - (zH * 256);

  ball.setColor(0,0,255);

  Serial.println("\nSKRIVER TIL EEPROM!");
  delay(100);
  Serial.println("--------------------------------------");
  delay(100);

  Serial.print("Ball-nummer:            ");
  delay(100);
  Serial.println(String(id_TOEEPROM, DEC));
  delay(100);
  EEPROM.write(ID,id_TOEEPROM);
  delay(100);
  Serial.print("SJEKKER... ");
  delay(100);
  if(id_TOEEPROM == EEPROM.read(ID)) {
    Serial.println("OK!");
    delay(100);
  } 
  else {
    Serial.println("FEIL!");
    delay(100);
  }
  ball.setColor(0,255,0);
  showInfo(); 
}

void setMax() {
  for (int i = 0; i < 60; i++) {
    if(Xcount[Xmax] < Xcount[i]) {
      Xmax = i;
    }
    if(Ycount[Ymax] < Ycount[i]) {
      Ymax = i;
    }
    if(Zcount[Zmax] < Zcount[i]) {
      Zmax = i;
    }
  }
}

void initCounters() {
  for (int i = 0; i < 60; i++) {
    Xcount[i] = 0;
    Ycount[i] = 0;
    Zcount[i] = 0;
  }
}

void defaultColor() {
  ball.setColor(1,1,1);
}







