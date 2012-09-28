/*
 * (c) 2012 Smarte baller med visuell forsterkning
 * <smarteballer@ifi.uio.no> http://www.smarteballer.no/
 */

#include <Ball2.h>


const byte Ball2::_simpleColorR[256] = {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,251,245,240,234,228,222,215,209,203,197,192,186,179,173,167,161,155,149,144,138,132,126,120,113,107,101,96,89,83,77,71,65,59,53,48,42,36,30,24,17,11,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,11,17,23,29,35,41,47,53,60,66,72,78,84,90,96,101,107,113,119,125,131,137,143,149,156,162,168,174,180,186,192,197,203,209,215,221,227,233,239,245,252,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};
const byte Ball2::_simpleColorG[256] = {0,6,12,18,24,30,36,42,48,54,60,65,72,78,84,89,96,102,108,113,120,126,131,138,144,150,156,162,168,174,179,186,192,198,204,210,216,222,227,234,240,246,252,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,251,245,240,234,227,221,215,209,203,197,192,186,179,173,167,161,155,149,144,138,131,125,119,113,107,101,96,90,83,77,71,65,59,53,48,42,35,29,23,17,11,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const byte Ball2::_simpleColorB[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,12,18,24,30,36,42,48,54,60,66,72,78,84,90,96,102,108,114,120,126,132,138,144,150,156,162,168,174,180,186,192,198,204,210,216,222,228,234,240,246,252,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,251,245,240,234,228,222,216,209,203,197,191,185,179,173,167,161,155,149,144,138,132,126,120,113,107,101,95,89,83,77,71,65,59,53,48,42,36,30,24,17,11,5,0};

// Constructor for Ball
Ball2::Ball2() {
  	pinMode(BLUEPIN, OUTPUT);
  	pinMode(REDPIN, OUTPUT);
  	pinMode(GREENPIN, OUTPUT);

	// Read from EEPROM.
  	_xN = (EEPROM.read(xEepromHigh) * 256) + EEPROM.read(xEepromLow);
  	_yN = (EEPROM.read(yEepromHigh) * 256) + EEPROM.read(yEepromLow);
  	_zN = (EEPROM.read(zEepromHigh) * 256) + EEPROM.read(zEepromLow);
  	_id = EEPROM.read(ID);

	// Read from EEPROM
	if (EEPROM.read(SPEAKER) == 1) {
		_hasSpeaker = TRUE;
	} else {
		_hasSpeaker = FALSE;
	}

	// Variables. Default _oneG is about 103
	_oneG = 102.0;
	_V = 0.0;
	_P = 0.0;

	// Force history
	_forceHistoryPointer = 0;
	_lastFreeFallPointer = 0;

	_brightness = 255;
	_targetBrightness = 255;
	_fadeSpeed = 1;
}

void Ball2::processAD(void) {
  	static int count;
  	static int identicalFs = 0;
  	static long longX, longY, longZ;

	// Read the analog port for the accelerometer values, and
	// subtract the value for 0 g to get force.
	_x = analogRead(X) - _xN;
  	_y = analogRead(Y) - _yN;
  	_z = analogRead(Z) - _zN;

  	// Put into a long variable before calculating force.
  	longX = _x;
  	longY = _y;
  	longZ = _z;

	// Store the previous force acting on the ball.
	_prevF = _F;

	// Calculate the force acting on the ball in this timestep.
	_F = (int) sqrt (longX * longX + longY * longY + longZ * longZ);

	// Force history
	_forceHistory [_forceHistoryPointer] = _F - _oneG;
	_forceHistoryPointer++;
	if (_forceHistoryPointer >= 100) {
		_forceHistoryPointer = 0;
	}

	// Calculate the difference in force.
	_diffF = abs(_F - _prevF);

	// Auto-calibration of _oneG and reset velocity.
	if (abs(102.0 - _F) < 4) {
		identicalFs++;
		// If we have read 30 identical measurements, reset and calibrate.
		if ((identicalFs >= 30)) {
			// _oneG = _F;
			_V = 0.0;
			// _P = 0.0; Leave _P untouched, because it
			// does not make sense to reset it.
			identicalFs = 0;
		}
	} else {
		identicalFs = 0;
	}

	// Numerical integration; the new velocity. 50 determines the slope.
	_V = _V + ((_F - _oneG)/50);

	// Numerical integration; the new position. 500 determines the slope.
	_P = _P + (_V/50);

	// Insted of calculating the force on the ball, we can examine
	// the force in each axis to determine free-fall.
	_absX = abs(_x);
	_absY = abs(_y);
	_absZ = abs(_z);

	// Store previous value.
	_prevSum = _sum;

	//Find sum of forces.
	_sum = _absX + _absY + _absZ;

	// Increment counter.
	count++;

	// We can now determine if the ball has been thrown or landed.
	// Observe that we need two consecutive measures of same state
	// before we actually switch. This is to prevent misreadings
	// in case of a noisy reading. When the ball is thrown or
	// lands we store the _holdtime or _flytime. It is useful for
	// many things.
 	if (_sum < _LIMIT && _prevSum < _LIMIT && !_inAir && count > 20) {
		_thrown = TRUE;
		_inAir = TRUE;
		_holdTime = count;
		count = 0;
	} else if (_sum > _LIMIT && _prevSum > _LIMIT && _inAir && count > 20) {
		_inAir = FALSE;
		_landed = TRUE;
		_flyTime = count;
		count = 0;
		_lastFreeFallPointer = _forceHistoryPointer;
	}

	// Try to determine if the ball has been tapped.
  	if (!_inAir && _diffF > 80 && count > 20) {
  		_tapped = TRUE;
		count = 0;
  	}

	if (_brightness != _targetBrightness) {
		if (_brightness < _targetBrightness) {
			_brightness = _brightness + _fadeSpeed;
			if (_brightness > _targetBrightness) {
				_brightness = _targetBrightness;
			}
		} else {
			_brightness = _brightness - _fadeSpeed;
			if (_brightness < _targetBrightness) {
				_brightness = _targetBrightness;
			}
		}
		setSimpleColor(_simpleColor);
	}
}

//The function aboves takes some time to execute. In the cases where
//we need a higher sampling frequency, we can use this function.
void Ball2::processADFast(void) {

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

  	if(diffX > 2 || diffY > 2 || diffZ > 2) {
  		_tapped = true;
  	}
}

// Interface to the _tapped value. Will return true if the ball has been
// tapped or moved since last time the variable was reset.
void Ball2::resetTapped(void) {
	_tapped = false;
}

// resets the movement or tap variable of the ball.
bool Ball2::getTapped(void) {
	return _tapped;
}

// Sets the color of the ball. RGB values from 0-255 are input.
void Ball2::setColor(byte R, byte G, byte B) {
	analogWrite(REDPIN, R);
	analogWrite(GREENPIN, G);
	analogWrite(BLUEPIN, B);
}

// Sets the color of the ball. Simple color C is input.
void Ball2::setSimpleColor(int C) {
	_simpleColor = C;
	analogWrite(REDPIN, _simpleColorR[C] * _brightness / 256);
	analogWrite(GREENPIN, _simpleColorG[C] * _brightness / 256);
	analogWrite(BLUEPIN, _simpleColorB[C] * _brightness / 256);
}

// Sets the SiteSwap color of the ball. SiteSwap number N is input.
void Ball2::setSiteSwapColor(byte N) {
	switch (N) {
	case 0:
		setColor(1,1,1); // DIM
		break;
	case 1:
		setSimpleColor(0); // RED
		break;
	case 2:
		setSimpleColor(42); // RED
		break;
	case 3:
		setSimpleColor(85); // GREEN
		break;
	case 4:
		setSimpleColor(171); // BLUE
		break;
	case 5:
		setSimpleColor(215); // PURPLE
		break;
	case 6:
		setSimpleColor(128); // CYAN ? (GREEN + BLUE)
		break;
	default:
		setColor(1,1,1); // DIM
		break;
	}
}

// Returns the force acting on the ball in the x-direction. Used in
// physics experiments.
int Ball2::getX() {
	return _x;
}

// Returns the force acting on the ball in the y-direction. Used in
// physics experiments.
int Ball2::getY() {
	return _y;
}
// Returns the force acting on the ball in the z-direction. Used in
// physics experiments.
int Ball2::getZ() {
	return _z;
}

// Returns the force acting on the ball. Used in physics experiments.
int Ball2::getF() {
	return _F;
}

// Returns the relative velocity of the ball.
int Ball2::getV(){
	return (int) _V;
}

// Returns the relative position
int Ball2::getP(){
	return (int) _P;
}

// Returns the relative position
int Ball2::getOneG(){
	return (int) _oneG;
}

//Returns the sum of the force acting on the ball.
int Ball2::getSum() {
	return _sum;
}

//Returns the RSSI.
int Ball2::getRSSI() {
	_rssi = analogRead(RSSI);
	return _rssi;
}

// Returns how long the ball was hold, before last throw.
int Ball2::getHoldTime() {
	return _holdTime;
}

// Returns how long the ball was in the air, last time it landed.
int Ball2::getFlyTime() {
	return _flyTime;
}

// Returns the state of the ball: true if the ball is in free fall.
bool Ball2::getInAir() {
	return _inAir;
}

// Returns true when the ball lands.
bool Ball2::getLanded() {
	return _landed;
}

// Resets the landed variable.
void Ball2::resetLanded() {
	_landed = FALSE;
}

//  Returns true when the balls gets thrown.
bool Ball2::getThrown() {
	return _thrown;
}

// Resets the thrown vaiable.
void Ball2::resetThrown() {
	_thrown = FALSE;
}

//  Returns true if the ball has a speaker.
bool Ball2::hasSpeaker() {
	return _hasSpeaker;
}


// Returns the battery level in volts. Observe that the the
// microcontroller runs from a power source regulated to 3.3 V.
// However, a fully charged battery will hold up to 4.2 V. As we do
// not want to put a voltage into a analogue pin higher than the VCC
// and AREF, we have divied the battery level by 2 (by means of r8 and
// r9 in the schematic) before connection to analog pin. Thus, we use
// the value _voltsPrBitDivided.
float Ball2::getBatteryLevel() {
	float temp;
	temp = analogRead(VCC);
	temp = temp * _voltsPrBitDivided;
	return temp;
}

// Returns the unique id of the ball (stored in EEPROM).
int Ball2::getId() {
	return _id;
}

// Force integral. Ignore first 5 values after landing.

int Ball2::getForceIntegral() {
	int forceIntegral = 0;
	_lastFreeFallPointer = (_lastFreeFallPointer + 5) % 100;
	if (_lastFreeFallPointer < _forceHistoryPointer) {
		for (int i = _lastFreeFallPointer; i < _forceHistoryPointer; i++) {
			forceIntegral += _forceHistory[i];
		}
	} else {
		for (int i = _lastFreeFallPointer; i < 100; i++) {
			forceIntegral += _forceHistory[i];
		}
		for (int i = 0; i < _forceHistoryPointer; i++) {
			forceIntegral += _forceHistory[i];
		}
	}
	return forceIntegral;
}

void Ball2::setBrightness(byte I) {
	_brightness = I;
	setSimpleColor(_simpleColor);
}

byte Ball2::getBrightness() {
	return _brightness;
}

void Ball2::setTargetBrightness(byte I) {
	_targetBrightness = I;
}

void Ball2::setFadeSpeed(byte I) {
	_fadeSpeed = I;
}

