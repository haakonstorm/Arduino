/*
 * (c) 2012 Smarte baller med visuell forsterkning
 * <smarteballer@ifi.uio.no> http://www.smarteballer.no/
 */

#ifndef Ball_h
#define Ball_h

#include <Arduino.h>
#include <EEPROM.h>
#include <MsTimer2.h>

// Pins for LEDS
#define REDPIN 10
#define GREENPIN 9
#define BLUEPIN 6

// Boolean constants
#define TRUE 1
#define FALSE 0

// Analog inputs
#define X A0
#define Y A1
#define Z A2
#define RSSI A3
#define VCC A4

// EEPROM addresses
#define xEepromHigh 0
#define xEepromLow 1
#define yEepromHigh 2
#define yEepromLow 3
#define zEepromHigh 4
#define zEepromLow 5
#define ID 6
#define SPEAKER 7

class Ball2{

public:

	Ball2(void);
	void processAD(void);
	void processADFast(void);
	void setColor(byte R, byte G, byte B);
	void setSimpleColor(int C);
	void setSiteSwapColor(byte C);
	int getX();
	int getY();
	int getZ();
	int getF();
	int getV();
	int getP();
	int getOneG();
	int getSum();
	int getRSSI();
	void resetTapped();
	bool getTapped();
	int getHoldTime();
	int getFlyTime();
	bool getInAir();
	float getBatteryLevel();
	int getId();
	bool getLanded();
	void resetLanded();
	bool getThrown();
	void resetThrown();
	bool hasSpeaker();
	int getForceIntegral();
	void setBrightness(byte I);
	byte getBrightness();
	void setTargetBrightness(byte I);
	void setFadeSpeed(byte I);
	void fadeToTargetBrightness();

private:

	void inits();

	// Numerical constants used for battery level etc
	static const float _voltsPrBit =  0.00322265f;
	static const float _voltsPrBitDivided = 0.0064453f;

	// Value for force-threshold flying / resting
	static const int _LIMIT = 15;

	// Private variables. These are typical sensor readings and
	// timer values gathered in the interrupt service routine and
	// utilized elsewhere
	bool _landed;
	byte _simpleColor;
	int _x, _y, _z, _rssi;
	float _oneG, _V, _P;
	int _absX, _absY, _absZ;
	int _sum, _prevSum;
	int _F, _prevF;
	int _diffF;
	bool _inAir;
	bool _thrown;
	bool _tapped;
	int _flyTime; // in number of samples; at 200Hz it is 5ms per sample

	int _holdTime; // in number of samples; at 200Hz it // is 5ms per sample
	int _xN, _yN, _zN; // Neutral values for x, y and z.
				    // Gathered from EEPROM in
				    // constructor.
	int _id;
	bool _hasSpeaker;
	int _forceHistory[100];
	int _forceHistoryPointer;
	int _lastFreeFallPointer;

	static const byte _simpleColorR[256];
	static const byte _simpleColorG[256];
	static const byte _simpleColorB[256];

	int _brightness;
	byte _targetBrightness;
	byte _fadeSpeed;
};

#endif
