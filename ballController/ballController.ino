#include <MsTimer2.h>

#define SLIDER_R 2
#define SLIDER_G 1
#define SLIDER_B 0
#define BUTTON1  10
#define LED1  5
#define LED2  6

#define PASSIVE 0
#define FORCE_GAME 1
#define SITESWAP 2

byte state = 1;
static int counter = 0;
unsigned char balls [4] = {
  0,5,6,7};

// table of exponential values
// generated for values of i from 0 to 255 -> x=round( pow( 2.0, i/32.0) - 1);
const byte expcolor[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 
  3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 
  5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7,
  7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 10, 10,
  10, 11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15,
  15, 15, 16, 16, 16, 17, 17, 18, 18, 18, 19, 19, 20, 20, 21, 21, 
  22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 28, 28, 29, 30, 30,
  31, 32, 32, 33, 34, 35, 35, 36, 37, 38, 39, 40, 40, 41, 42, 43,
  44, 45, 46, 47, 48, 49, 51, 52, 53, 54, 55, 56, 58, 59, 60, 62,
  63, 64, 66, 67, 69, 70, 72, 73, 75, 77, 78, 80, 82, 84, 86, 88, 
  90, 91, 94, 96, 98, 100, 102, 104, 107, 109, 111, 114, 116, 119,
  122, 124, 127, 130, 133, 136, 139, 142, 145, 148, 151, 155, 158,
  161, 165, 169, 172, 176, 180, 184, 188, 192, 196, 201, 205, 210,
  214, 219, 224, 229, 234, 239, 244, 250
};

void setup(){
  Serial.begin(19200);
  MsTimer2::set(50, sendSignal);
  MsTimer2::start();
//  counter++;
//  digitalWrite(LED2,HIGH);
}

void sendSignal(){

  //if (state == REMOTE_CONTROL) {
    static byte val_R = 0;
    static byte val_G = 0;
    static byte val_B = 0;

    val_R = expcolor[map(analogRead(SLIDER_R),0,1023,255,0)];
    val_G = expcolor[map(analogRead(SLIDER_G),0,1023,255,0)];
    val_B = expcolor[map(analogRead(SLIDER_B),0,1023,255,0)];

    Serial.write('c');
    Serial.write(val_R);
    Serial.write(val_G);
    Serial.write(val_B);
  //}
}

void loop() {
} 

void loops() {
  if(!(digitalRead(BUTTON1))) {
    digitalWrite(LED1,HIGH);
    Serial.println("HEI");
    delay(1);
    state++;
    if(state > 2){ 
      state = 0;
    }

    if (state == PASSIVE) {
      Serial.write('.');
    } 
    else {
      Serial.write(':');
      Serial.write(state);
    }
    if (state == FORCE_GAME) {
      Serial.write((byte) balls[random(4)]);
    }

    while(!(digitalRead(BUTTON1)));
  }
}





