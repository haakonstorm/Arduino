/*
 * (c) 2011 Smarte baller med visuell forsterkning <smarteballer@ifi.uio.no>
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


// Define colors for different siteswap values:

#define GREEN 3
#define RED 4
#define CYAN 1
#define PURPLE 5
#define BLUE 2 //active 2. 
#define ORANGE 6
#define WHITE 7


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
    void processAD(void);
    void fadeColor(bool r, bool g, bool b);
    void setFadeColor(unsigned char color);
    void setReleaseColor(unsigned char R, unsigned char G, unsigned char B);
    void setColor(unsigned char R, unsigned char G, unsigned char B);
    void radioBroadcast(void);
    int getX();
    int getY();
    int getZ();
    int getF();
    int getV();
    int getP();
    int getRSSI();
    int getSum();
    unsigned int getHoldTime();
    unsigned int getFlyTime();
    bool getInAir();
    float getBatteryLevel();
    unsigned char getId();		
    bool getLanded();
    void resetLanded();
    bool getThrown();
    void resetThrown();
    void delayedSetColor(unsigned char R, unsigned char G, unsigned char B);
    static const byte expcolor [256];
    
	// PRIVATE
private:	
    // Numerical constants used for battery level etc
    static const float _voltsPrBit =  0.00322265f;
    static const float _voltsPrBitDivided = 0.0064453f;
    // Value for force-threshold fluing / resting
    static const unsigned int _LIMIT = 15;
    
    // global variables. These are typical sensor readings and timer values gathered
    // in the interrupt service routine and utilized elsewhere
    bool _landed;
    int _x, _y, _z;
    int _rssi;
    float _oneG, _V, _P;
    unsigned int _absX, _absY, _absZ; 
    unsigned int _sum, _prevSum;
    float _F, _prevF;
    bool _inAir;
    bool _thrown;
    unsigned int _flyTime;	// in number of samples; at 200Hz it is 5ms per sample
    unsigned int _holdTime;	// ditto
    unsigned int _halfHoldTime; //used for changing color between two throws.
    unsigned int _xN, _yN, _zN; // Neutral values for x, y and z. Gathered from EEPROM in constructor.
    //Siteswap value for the ball. Will increase with each incomming value.
    unsigned char _id;
	
};

#endif