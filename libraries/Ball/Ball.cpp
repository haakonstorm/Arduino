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
  	pinMode(BLUEPIN, OUTPUT);
  	pinMode(REDPIN, OUTPUT);
  	pinMode(GREENPIN, OUTPUT);
      
  	_xN = (EEPROM.read(xEepromHigh) * 256) + EEPROM.read(xEepromLow);
  	_yN = (EEPROM.read(yEepromHigh) * 256) + EEPROM.read(yEepromLow);
  	_zN = (EEPROM.read(zEepromHigh) * 256) + EEPROM.read(zEepromLow);
  	_id = EEPROM.read(ID); 
}

void Ball::processAD(void){
  	
  	static unsigned int count;
  	static long longX, longY, longZ;

	_x = analogRead(X) - _xN;
  	_y = analogRead(Y) - _yN;
  	_z = analogRead(Z) - _zN;
  	
  	longX = _x;
  	longY = _y;
  	longZ = _z;
  	
    _prevF = _F;
	_F = (int) sqrt (longX * longX + longY * longY + longZ * longZ);
	
	_absX =abs(_x);
	_absY =abs(_y);
	_absZ =abs(_z);
	
	_prevSum = _sum;
	_sum = _absX + _absY + _absZ;
	 	
	count ++;	
   
 	if (_sum < _LIMIT && _prevSum < _LIMIT && !_inAir){
    	_inAir = TRUE;
      	_holdTime = count;
      	_halfHoldTime = (int) count /2;
      	count = 0;
    } else if (_sum > _LIMIT && _prevSum > _LIMIT && _inAir){
    	_inAir = FALSE;
    	_flyTime = count;
    	count = 0;
    	landed = TRUE;
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


void Ball::delayedSetColor(unsigned char R, unsigned char G, unsigned char B){
	static char cR=0,cG=0,cB=0;
	static int stepCounter = 0;
	if (R == cR && G == cG && B ==cB){ 	// the color is correct.
		return;
	}else{
		stepCounter ++;
		if(stepCounter >= _halfHoldTime){
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
	return _F;
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
	return landed;
}

void Ball::resetLanded(void){
	landed = FALSE;
}

float Ball::getBatteryLevel(){
	float temp;
	temp = analogRead(VCC);
	temp = temp * _voltsPrBitDivided;
	return temp;
}

unsigned int Ball::getId(){
	return _id;
}