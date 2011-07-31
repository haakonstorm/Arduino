/*
 * (c) 2011 Smarte baller med visuell forsterkning <smarteballer@ifi.uio.no>
 * http://www.smarteballer.no/
 */

#ifndef Ball_h
#define Ball_h

#include "WProgram.h"

// First, define pins for LEDS:
#define REDPIN 10
#define GREENPIN 9 
#define BLUEPIN 6

// boolean constants
#define TRUE 1
#define FALSE 0


// Define colors for different siteswap values:

#define GREEN 3
#define RED 4
#define CYAN 1
#define PURPLE 5
#define BLUE 2 //active 2. 

// Then, define analog inputs from accelerometer, RSSI and VCC/2:
#define X A0
#define Y A1
#define Z A2
#define RSSI A3
#define VCC A4

// EEPROM adresses for ball calibration and ID:
#define xEepromHigh 0
#define xEepromLow 1
#define yEepromHigh 2
#define yEepromLow 3
#define zEepromHigh 4
#define zEepromLow 5
#define ID 6

class Ball{

	// PUBLIC
	public:

		Ball(void);
		int x, y, z, sumF, F;
		void processAD(void);
		void colorFade(void);
		void setColor(char R, char G, char B);
		void report(void);
		void detectPattern (unsigned char siteswapValue);
		void predictThrow(void);
		bool getLanded();
		unsigned char getSiteswap();
			
	// PRIVATE
	private:
	
		// Numerical constants used for battery level etc
		static const float _voltsPrBit =  0.00322265f;
		static const float _voltsPrBitDivided = 0.0064453f;
		// Value for force-threshold fluing / resting
		static const unsigned int _LIMIT = 15;

		// global variables. These are typical sensor readings and timer values gathered
		// in the interrupt service routine and utilized elsewhere


		bool _inAir;
		unsigned int _flyTime;	// in number of samples; at 200Hz it is 5ms per sample
		unsigned int _holdTime;	// ditto
		unsigned char _color;	// used by colorFade to set new color.
		unsigned int _xN, _yN, _zN; // Neutral values for x, y and z. Gathered from EEPROM in constructor.
		//Siteswap value for the ball. Will increase with each incomming value.
		unsigned char _siteswap; 
	
		//semaphores:
		bool _patternLock; // is the pattern locked? if so predict value at own landing, and check of the pattern is broken based on predictions.
		bool _running; //means that the balls are in juggling use. 
		bool _landed;

#endif