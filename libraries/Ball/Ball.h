#ifndef Ball_h
#define Ball_h

#include "WProgram.h"

// First, define pins for LEDS:
#define RED 10
#define GREEN 9 
#define BLUE 6

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
	Ball(void);
	void processAD(void);
	void colorFade(void);
	int x;
	int y;
	int z;
	
	
	
	private:
    // Numerical constants used for battery level etc 
	static const float voltsPrBit =  0.00322265f;
	static const float voltsPrBitDivided = 0.0064453f;

	// Value for force-threshold fluing / resting
	static const unsigned int LIMIT = 15;

	// global variables. These are typical sensor readings and timer values gathered
	// in the interrupt service routine and utilized elsewhere
	
	unsigned char inAir;
	unsigned int flyTime; //in samples of 5ms ea.
	unsigned int holdTime;  // ditto
	unsigned char color; //used by colorFade to set new color.

	unsigned int xN, yN, zN;
};



#endif