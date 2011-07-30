#ifndef Ball_h
#define Ball_h

#include "WProgram.h"

// First, define pins for LEDS:
#define RED 6
#define GREEN 9 
#define BLUE 10

// Then define analog inputs from accelerometer, RSSI and VCC/2

#define X A0
#define Y A1
#define Z A2
#define RSSI A3
#define VCC A4

// EEPROM adresses for ball calibration and ID.
#define xEepromHigh 0
#define xEepromLow 1
#define yEepromHigh 2
#define yEepromLow 3
#define zEepromHigh 4
#define zEepromLow 5
#define ID 6

class Ball{
	public:
	void readAD(void);
	
	
	private:
    // Numerical constants used for battery level etc 
	const float voltsPrBit = 0.00322265f;
	const float voltsPrBitDivided = 0.0064453f;

	// Value for force-threshold fluing / resting
	const unsigned int LIMIT = 15;

	// global variables. These are typical sensor readings and timer values gathered
	// in the interrupt service routine and utilized elsewhere
	unsigned int xN, yN, zN;

	unsigned char inAir;
	unsigned int flyTime; //in samples of 5ms ea.
	unsigned int holdTime;  // ditto
	unsigned char color; //used by colorFade to set new color.

	unsigned int xN, yN, zN;



}



#endif