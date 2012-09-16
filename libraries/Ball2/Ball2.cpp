/*
 * (c) 2012 Smarte baller med visuell forsterkning <smarteballer@ifi.uio.no>
 * http://www.smarteballer.no/
 */

#include <EEPROM.h>
#include <MsTimer2.h>
#include <Ball2.h>

// Constructor for Ball
Ball2::Ball2(){
  	pinMode(BLUEPIN, OUTPUT);
  	pinMode(REDPIN, OUTPUT);
  	pinMode(GREENPIN, OUTPUT);
      
  	_xN = (EEPROM.read(xEepromHigh) * 256) + EEPROM.read(xEepromLow);
  	_yN = (EEPROM.read(yEepromHigh) * 256) + EEPROM.read(yEepromLow);
  	_zN = (EEPROM.read(zEepromHigh) * 256) + EEPROM.read(zEepromLow);
  	_id = EEPROM.read(ID);
  	
  	}

void Ball2::processAD(void){
  	
  	static unsigned int count;
  	static long longX, longY, longZ;
  	
	// read the analog port for the accelerometer values, and subtract the value for 0 g to get force.
	_x = analogRead(X) - _xN;
  	_y = analogRead(Y) - _yN;
  	_z = analogRead(Z) - _zN;
  	
  	//Put into a long variable before calculating force.
  	longX = _x;
  	longY = _y;
  	longZ = _z;

	//Store the previous force acting on the ball.  	
    _prevF = _F;
    
    //calculate the force acting on the ball in this timestep.
	_F = (int) sqrt (longX * longX + longY * longY + longZ * longZ);

	// insted of calculating the force on the ball, we can examine the force in each axis to determine free-fall:
	_absX =abs(_x);
	_absY =abs(_y);
	_absZ =abs(_z);
	// Store previous value.
	_prevSum = _sum;
	
	//Find sum of forces.
	_sum = _absX + _absY + _absZ;
	
	count++;
   // We can now determine if the ball has been thrown or landed. Observe that we need two consecutive measures of same state before we actually 
   // switch. This is to prevent misreadings in case of a noisy reading. When the ball is thrown or lands we store the _holdtime or _flytime. It
   // is useful for many things.
 	if (_sum < _LIMIT && _prevSum < _LIMIT && !_inAir && count > 20){
    	_thrown = TRUE;
      	_inAir = TRUE;
      	_holdTime = count;
      	count = 0;
    } else if (_sum > _LIMIT && _prevSum > _LIMIT && _inAir count > 20){
    	_inAir = FALSE;
    	_landed = TRUE;
    	_flyTime = count;
    	count = 0;
    }
}

//The function aboves takes some time to execute. In the cases where we need a higher sampling frequence, we can use this function.

void Ball2::processADFast(void){
  	
  	static int prevX, prevY, prevZ, diffX, diffY, diffZ;

	prevX = _x;
	prevY = _y;
	prevZ = _z;
	
	_x = analogRead(X);
  	_y = analogRead(Y);
  	_z = analogRead(Z);
  	
  	diffX = prevX - _x;
  	diffY = prevY - _y;
  	diffZ = prevZ - _z;
  	
  	if(diffX > 2 || diffY > 2 || diffZ > 2){
  		_tap = true;
  	}
  	
}

// interface to the _tap value. Will return true of the ball has been tapped or moved since last time the variable was reset.
void Ball2::resetTap(void){
	_tap = false;

}

// resets the movement or tap variable of the ball.
bool Ball2::getTap(void){
	return _tap;

}


// Sets the color of the ball. RGB values from 0-255 are input.
void Ball2::setColor(unsigned char R, unsigned char G, unsigned char B){
	analogWrite(REDPIN, R);
	analogWrite(GREENPIN, G);
	analogWrite(BLUEPIN, B);
}

//	Returns the force acting on the ball in the x-direction. Used in physics experiments.
int Ball2::getX(){
	return _x;
}

// Returns the force acting on the ball in the y-direction. Used in physics experiments.
int Ball2::getY(){
	return _y;
}
// Returns the force acting on the ball in the z-direction. Used in physics experiments.
int Ball2::getZ(){
	return _z;
}

// Returns the force acting on the ball. Used in physics experiments.
int Ball2::getF(){
	return _F;
}

//Returns the sum of the force acting on the ball.
int Ball2::getSum(){
	return _sum;
}

//Returns the RSSI
int Ball2::getRSSI(){
	_rssi = analogRead(RSSI);
	return _rssi;
}

// Returns how long the ball was hold, before last throw
unsigned int Ball2::getHoldTime(){
	return _holdTime;
}
	
// Returns how long the ball was in the air, last time it landed.
unsigned int Ball2::getFlyTime(){
	return _flyTime;
}

// returns the state of the ball, true if the ball is in free fall.
bool Ball2::getInAir(){
	return _inAir;
}

// returns true when the ball lands
bool Ball2::getLanded(){
	return _landed;
}

// reset the landed variable.
void Ball2::resetLanded(){
	_landed = FALSE;
}

// Retruns true when the balls gets thrown
bool Ball2::getThrown(){
	return _thrown;
}

// resets the thrown vaiable.
void Ball2::resetThrown(){
	_thrown = FALSE;
}

// Returns the battery level in volts. Observe that the the microcontroller runs from a power source regulated to 3.3 v. 
// However, a fully charged battery will hold up to 4.2 volts. As we do not want to put a voltage into a analogue pin higher than the Vcc and aRef,
// we have divied the battery level by two (By means of r8 and r9 in the schematic) before connection to analogue pin. 
// Thus, we use the value _voltsPrBitDivided.
float Ball2::getBatteryLevel(){
	float temp;
	temp = analogRead(VCC);
	temp = temp * _voltsPrBitDivided;
	return temp;
}

// Returns the unique id of the ball (Stored in eeprom).
unsigned int Ball2::getId(){
	return _id;
}