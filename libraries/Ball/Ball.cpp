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
    
    // Pins for colors
  	pinMode(BLUEPIN, OUTPUT);
  	pinMode(REDPIN, OUTPUT);
  	pinMode(GREENPIN, OUTPUT);

    // Read constants from EEPROM
  	_xN = (EEPROM.read(xEepromHigh) * 256) + EEPROM.read(xEepromLow);
  	_yN = (EEPROM.read(yEepromHigh) * 256) + EEPROM.read(yEepromLow);
  	_zN = (EEPROM.read(zEepromHigh) * 256) + EEPROM.read(zEepromLow);
  	_id = EEPROM.read(ID);
    
    // Variables
    _oneG = 103.0;
    _V = 0.0;
    _P = 0.0;

  	
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
}

void Ball::processAD(void){
  	
    static unsigned int count;
  	static unsigned int identicalFs = 0;
  	static long longX, longY, longZ;
    
	_x = analogRead(X) - _xN;
  	_y = analogRead(Y) - _yN;
  	_z = analogRead(Z) - _zN;
  	
  	longX = _x;
  	longY = _y;
  	longZ = _z;
  	
    _prevF = _F;
    _F = sqrt (longX * longX + longY * longY + longZ * longZ);
    
    if (abs(103 - _F) < 2) {
        identicalFs++;
        if ((identicalFs >= 10)) {
            _oneG = _F;
            _V = 0.0;
            _P = 0.0;
            identicalFs = 0;
        }
    } else {
        identicalFs = 0;
    }
    
    
    
	
	_V = _V + ((_F - _oneG)/20);
    
    _P = _P + (_V/20);
    
	_absX =abs(_x);
	_absY =abs(_y);
	_absZ =abs(_z);
	
	_prevSum = _sum;
	_sum = _absX + _absY + _absZ;
    
	count ++;	
    
 	if (_sum < _LIMIT && _prevSum < _LIMIT && !_inAir){
    	_thrown = TRUE;
      	_inAir = TRUE;
      	_holdTime = count;
      	_halfHoldTime = (int) count /2;
      	count = 0;
    } else if (_sum > _LIMIT && _prevSum > _LIMIT && _inAir){
    	_inAir = FALSE;
    	_landed = TRUE;
    	_flyTime = count;
    	count = 0;
    }
}

// fades from previos color to the new one in 200 ms. (i.e. 40 samples) called from isr.
void Ball::fadeColor (bool r, bool g, bool b){ 
	static int strength  = 255;
	static bool decrease = TRUE;
	static bool cR = TRUE, cG = TRUE, cB = TRUE;
	if (r == cR && g == cG && b ==cB){ //the ball is illuminated with the correct color
		return;
	}else{
		if(decrease){
			strength = strength - 13;
			if(strength<=0){
				strength = 0;
				decrease = false;
				return;
            }
			if(cR)
				analogWrite(REDPIN, strength);
			if(cG)
				analogWrite(GREENPIN, strength);
			if(cB)
				analogWrite(BLUEPIN, strength);
		}else{
			strength = strength + 13;
			if(r)
				analogWrite(REDPIN, strength);
			if(g)
				analogWrite(GREENPIN, strength);
			if(b)
				analogWrite(BLUEPIN, strength);
			if(strength>=255){
				strength = 255;
				decrease = true;
				cR = r;
				cG = g;
				cB = b;	
			}
		}
	}
}

void Ball::setFadeColor(unsigned char color){
	if(color==1)
		fadeColor(FALSE,TRUE,TRUE);
	if(color==2)
		fadeColor(FALSE,FALSE,TRUE);
	if(color==3)
		fadeColor(FALSE, TRUE, FALSE);
	if(color==4)
		fadeColor(TRUE, FALSE, FALSE);
	if(color==5)
		fadeColor(TRUE,FALSE,TRUE);
	if(color==6)
		fadeColor(TRUE,TRUE,FALSE);
	if(color==7)
		fadeColor(TRUE,TRUE,TRUE);
}

void Ball::setColor(unsigned char R, unsigned char G, unsigned char B){
	analogWrite(REDPIN, R);
	analogWrite(GREENPIN, G);
	analogWrite(BLUEPIN, B);
}

void Ball::setReleaseColor(unsigned char newR, unsigned char newG, unsigned char newB){
	static char oldR = 0, oldG = 0, oldB = 0;
	if (newR == oldR && newG == oldG && newB ==oldB){ 	// the color is correct.
		return;
	} else {
		if(_inAir){
			oldR = newR;
			oldG = newG;
			oldB = newB;
			setColor(newR,newG,newB);
		}
	}	
}

void Ball::delayedSetColor(unsigned char R, unsigned char G, unsigned char B){
	static char cR=0,cG=0,cB=0;
	static int stepCounter = 0;
	if (R == cR && G == cG && B ==cB){ 	// the color is correct.
		return;
	} else {
		stepCounter ++;
		if(stepCounter >= _halfHoldTime + 25){
			cR=R;
			cG=G;
			cB=B;
			stepCounter = 0;
			setColor(R,G,B);
		}
	}	
}

void Ball::radioBroadcast(){
	// Serial.write(sumF);
}

int Ball::getX(){
	return _x;
}

int Ball::getY(){
	return _y;
}

int Ball::getZ(){
	return _z;
}

int Ball::getF(){
	return (int) _F;
}

int Ball::getV(){
	return (int) _V;
}

int Ball::getP(){
	return (int) _P;
}

int Ball::getSum(){
	return _sum;
}

unsigned int Ball::getHoldTime(){
	return _holdTime;
}

unsigned int Ball::getFlyTime(){
	return _flyTime;
}

bool Ball::getInAir(){
	return _inAir;
}

bool Ball::getLanded(){
	return _landed;
}

void Ball::resetLanded(){
	_landed = FALSE;
}

bool Ball::getThrown(){
	return _thrown;
}

void Ball::resetThrown(){
	_thrown = FALSE;
}

float Ball::getBatteryLevel(){
	float temp;
	temp = analogRead(VCC);
	temp = temp * _voltsPrBitDivided;
	return temp;
}

unsigned char Ball::getId(){
	return _id;
}