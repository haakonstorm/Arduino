/*
 * (c) 2011 Smarte baller med visuell forsterkning <smarteballer@ifi.uio.no>
 * http://www.smarteballer.no/
 */

#include <WProgram.h>
#include <EEPROM.h>
#include <MsTimer2.h>
#include <Ball.h>

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

  	pinMode(BLUEPIN, OUTPUT);
  	pinMode(REDPIN, OUTPUT);
  	pinMode(GREENPIN, OUTPUT);
      
  	_xN = (EEPROM.read(xEepromHigh) * 256) + EEPROM.read(xEepromLow);
  	_yN = (EEPROM.read(yEepromHigh) * 256) + EEPROM.read(yEepromLow);
  	_zN = (EEPROM.read(zEepromHigh) * 256) + EEPROM.read(zEepromLow);
}

void Ball::processAD(void){
  	
  	static int count = 0;

	_x = analogRead(X) - _xN;
  	_y = analogRead(Y) - _yN;
  	_z = analogRead(Z) - _zN;
  	
    _x = abs(_x);
    _y = abs(_y);
  	_z = abs(_z);
  	
   	_prevSum = _sum;
   	_sum = _x + _y + _z;
	_F = sqrt(_x*_x+_y*_y+_z*_z);
	 	
	count ++;	
   
    if (_sum > 255)
      _sum = 255;

 	if (_sum < _LIMIT && _prevSum < _LIMIT){
    	_inAir = TRUE;
      	_holdTime = count;
      	count = 0;
    } else {
    	_inAir = FALSE;
    	_flyTime = count;
    	count = 0;
    }
}


void Ball::detectPattern(unsigned char siteswapValue){
}

void Ball::colorFade (void){ // fades from previos color to the new one in 400 ms. (i.e. 80 samples) calles from isr.
}

void Ball::setColor(char R, char G, char B){
	analogWrite(RED, R);
	analogWrite(GREEN, G);
	analogWrite(BLUE, B);
}

void Ball::report(){
	// Serial.write(sumF);
}

void Ball::predictThrow(void){
}

bool Ball::getLanded(void){
	return _landed;
}

unsigned char Ball::getSiteswap(){
	return _siteswap;
}
