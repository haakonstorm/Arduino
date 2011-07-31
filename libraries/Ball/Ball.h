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
	public:
<<<<<<< HEAD
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
=======
		Ball(void);
		void processAD(void);
		void colorFade(void);
		void detectPattern (unsigned char siteswapValue);
		void predictThrow(void);
		bool getLanded();
		unsigned char getSiteswap();
	private:
	    // Numerical constants used for battery level etc 
		static const float voltsPrBit =  0.00322265f;
		static const float voltsPrBitDivided = 0.0064453f;
>>>>>>> added wrapper functions and variables

		// Value for force-threshold fluing / resting
		static const unsigned int LIMIT = 15;

<<<<<<< HEAD
	// Global variables. These are typical sensor readings and timer values gathered
	// in the interrupt service routine and utilized elsewhere.
=======
		// global variables. These are typical sensor readings and timer values gathered
		// in the interrupt service routine and utilized elsewhere
	
		bool inAir;
		unsigned int flyTime; //in samples of 5ms ea.
		unsigned int holdTime;  // ditto
		unsigned char color; //used by colorFade to set new color (we need a color convention).
	
		//Neutral values for x, y and z. Gathered from EEPROM in constructor.
		unsigned int xN, yN, zN;
	
		//Siteswap value for the ball. Will increase with each incomming value.
		unsigned char siteswap; 
	
		//semaphores:
		bool patternLock; // is the pattern locked? if so predict value at own landing, and check of the pattern is broken based on predictions.
		bool running; //means that the balls are in juggling use. 
		bool landed;
>>>>>>> added wrapper functions and variables
	
};

#endif