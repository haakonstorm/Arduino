
#include <Ball2.h>
#include <MsTimer2.h>
#include <EEPROM.h>

Ball2 ball;

void setup(){
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
	TCCR0B = TCCR0B & 0b11111000 | 0x05;
    
	// Pins 9 and 10  are controlled by 16-bit Timer/Counter1 in phase correct PWM mode.
	// Frequency = 16000000 / (510 * N) = 31372,549019608 / N
	// 0x01     1  31372,549019608
	// 0x02     8  3921,568627451
	// 0x03    64  490,196078431 (default)
	// 0x04   256  122,549019608
	// 0x05  1024  30,637254902	 	
	TCCR1B = TCCR1B & 0b11111000 | 0x05;


  Serial.begin(19200);
  MsTimer2::set(5, processAD);
  MsTimer2::start();
  ball.setColor(255,128,0);
}

void processAD(){
}

void loop(){
}


