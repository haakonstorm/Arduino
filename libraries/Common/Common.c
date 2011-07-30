#include<Common.h>

// Numerical constants used for battery level etc 

const float voltsPrBit = 0.00322265f;
const float voltsPrBitDivided = 0.0064453f;

// Value for force-threshold fluing / resting
const unsigned int LIMIT = 15; //May be reduced even further in the future, 

// Global variables. These are typical sensor readings and timer values gathered
// in the interrupt service routine and will be utilized elsewhere

unsigned char inAir;
unsigned int flyTime; //in samples of 5ms ea.
unsigned int holdTime;  // ditto
unsigned char color; //used by colorFade to set new color.

unsigned int xN, yN, zN;

// functions:
