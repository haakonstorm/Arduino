/*
 * (c) 2012 Smarte baller med visuell forsterkning <smarteballer@ifi.uio.no>
 * http://www.smarteballer.no/
 */

#ifndef Ball_h
#define Ball_h

#include "Arduino.h"

// First, define pins for LEDS:
#define REDPIN 10
#define GREENPIN 9 
#define BLUEPIN 6

// boolean constants
#define TRUE 1
#define FALSE 0

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

class Ball2{

	// PUBLIC
	public:
		Ball2(void);
		void processAD(void);
		void processADFast(void);
		void setColor(unsigned char R, unsigned char G, unsigned char B);
		int getX();
		int getY();
		int getZ();
		int getF();
		int getSum();
		int getRSSI();
		void resetTap();
		bool getTap();
		unsigned int getHoldTime();
		unsigned int getFlyTime();
		bool getInAir();
		float getBatteryLevel();
		unsigned int getId();		
		bool getLanded();
		void resetLanded();
		bool getThrown();
		void resetThrown();

	// PRIVATE
	private:	
		// Numerical constants used for battery level etc
		static const float _voltsPrBit =  0.00322265f;
		static const float _voltsPrBitDivided = 0.0064453f;
		// Value for force-threshold flying / resting
		static const unsigned int _LIMIT = 15;
		
		// privat ariables. These are typical sensor readings and timer values gathered
		// in the interrupt service routine and utilized elsewhere
		bool _landed; 
		int _x, _y, _z, _rssi;
		unsigned int _absX, _absY, _absZ; 
		unsigned int _sum, _prevSum;
		unsigned int _F, _prevF;
		bool _inAir;
		bool _thrown;
		bool _tap;
		unsigned int _flyTime;	// in number of samples; at 200Hz it is 5ms per sample
		unsigned int _holdTime;	// ditto
		unsigned int _xN, _yN, _zN; // Neutral values for x, y and z. Gathered from EEPROM in constructor.
		unsigned int _id;
	
};

#endif