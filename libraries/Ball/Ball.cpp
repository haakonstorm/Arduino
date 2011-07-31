#include <Ball.h>
#include <WProgram.h>
#include <EEPROM.h>

// Constructor for Ball
Ball::Ball(){
	
	// The voltage applied to the AREF pin (0 to 5V only) is used as the reference.
	analogReference(EXTERNAL);

	// Adjust Arduino PWM frequencies. See http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1235060559
	// Pins 5 and 6  are controlled by 8-bit Timer/Counter0 in fast PWM mode.
	// This affects millis() and delay().
	// Frequency = 16000000 / (256 * N) = 62500 / N
	// 0x01	   1	62500
	// 0x02	   8	7812,5
	// 0x03	  64	976,5625 (default)
	// 0x04	 256	244,140625
	// 0x05	1024	61,03515625
  	TCCR0B = TCCR0B & 0b11111000 | 0x02;
  	
  	// Pins 9 and 10  are controlled by 16-bit Timer/Counter1 in phase correct PWM mode.
  	// Frequency = 16000000 / (510 * N) = 31372,549019608 / N
	// 0x01	   1	31372,549019608
	// 0x02	   8	3921,568627451
	// 0x03	  64	490,196078431 (default)
	// 0x04	 256	122,549019608
	// 0x05	1024	30,637254902
	TCCR1B = TCCR1B & 0b11111000 | 0x02;

	// Common settings.
  	pinMode(RED, OUTPUT);
  	pinMode(GREEN, OUTPUT);
  	pinMode(BLUE, OUTPUT);
  	Serial.begin(19200);	
	digitalWrite(BLUE,1);
	
	// Reading normalized values.
  	xN = (EEPROM.read(xEepromHigh) * 256) + EEPROM.read(xEepromLow);
  	yN = (EEPROM.read(yEepromHigh) * 256) + EEPROM.read(yEepromLow);
  	zN = (EEPROM.read(zEepromHigh) * 256) + EEPROM.read(zEepromLow);
}

void Ball::processAD (void){
  	static int x,y,z;
  	static int sum;
  	static int count = 0;

	x = analogRead(X) - xN;
  	y = analogRead(Y) - yN;
  	z = analogRead(Z) - zN;
  	
  	x = abs(x);
    y = abs(y);
  	z = abs(z);
  
   	sum = x + y + z;
	
	count ++;	
   
    if(sum > 255)
      sum = 255;

    if(sum < LIMIT){
      digitalWrite(RED, 1);
      digitalWrite(GREEN, 0);
    }
    
    else{
      digitalWrite(GREEN, 1);
      digitalWrite(RED, 0);     
    }
}

void Ball::colorFade (void){ // fades from previos color to the new one in 400 ms. (i.e. 80 samples) calles from isr.

}
