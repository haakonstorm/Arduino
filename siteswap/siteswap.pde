#include <Ball.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball ball;

void setup(){
  Serial.begin(19200);
  // The voltage applied to the AREF pin (0 to 5V only) is used as the reference.
  analogReference(EXTERNAL);

 
// Adjust Arduino PWM frequencies. See http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1235060559
// Pins 5 and 6  are controlled by 8-bit Timer/Counter0 in fast PWM mode.
// This affects millis() and delay().
// Frequency = 16000000 / (256 * N) = 62500 / N
// 0x01     1  62500
// 0x02     8  7812,5
// 0x03    64  976,5625 (default)
// 0x04   256  244,140625
// 0x05  1024  61,03515625 
TCCR0B = TCCR0B & 0b11111000 | 0x02;
	 	
// Pins 9 and 10  are controlled by 16-bit Timer/Counter1 in phase correct PWM mode.
// Frequency = 16000000 / (510 * N) = 31372,549019608 / N
// 0x01     1  31372,549019608
// 0x02     8  3921,568627451
// 0x03    64  490,196078431 (default)
// 0x04   256  122,549019608
// 0x05  1024  30,637254902	 	
TCCR1B = TCCR1B & 0b11111000 | 0x02;


  MsTimer2::set(5, processAD); // 5ms period, 200Hz
  MsTimer2::start();
}



void processAD(){
  ball.processAD(); 
//  ball.setFadeColor(color);

}

int siteswap=1;
void loop(){


  if(Serial.available()) {
    Serial.flush();
    siteswap++;
  }


  if(ball.getLanded()){
    ball.resetLanded();
    Serial.print(siteswap); 
    switch (siteswap){
      case 1:
        ball.setColor(255,0,0);
        break;
      
      case 2:
        ball.setColor(0,0,255);
        break;
      
      case 3:
        ball.setColor(0,255,0);
        break;
      
      case 4:
        ball.setColor(128,128,0);
        break;
      
      case 5:
        ball.setColor(128,0,128);
        break;
      
      case 6:
        ball.setColor(0,128,128);
        break;
    }
      
    siteswap=1;
  }
  
}







