#include <Ball.h>
#include <EEPROM.h>
#include <WProgram.h>
// Numerical constants used for battery level etc 


Ball::Ball(){ //Constructor.
	
  	xN = (EEPROM.read(xEepromHigh) * 256) + EEPROM.read(xEepromLow);
  	yN = (EEPROM.read(yEepromHigh) * 256) + EEPROM.read(yEepromLow);
  	zN = (EEPROM.read(zEepromHigh) * 256) + EEPROM.read(zEepromLow);
}

void Ball::processAD (void){
  	static int x,y,z;
  	static int sum;
  	static int count = 0;

	x = analogRead(X) - xN;
  	y = analogRead(Y) - yN;
  	z = analogRead(Z) - zN;
  	
  	x=abs(x);
    y=abs(y);
  	z=abs(z);
  
   	sum = x + y + z;
	
	count ++;	
   
    if(sum>255)
      sum=255;

    if(sum < LIMIT){
      digitalWrite(RED, 1);     
      digitalWrite(GREEN, 0);     
    }
    
    else{
      digitalWrite(GREEN, 1);
      digitalWrite(RED, 0);     
    }

Serial.print("X:");
Serial.print(x);
Serial.print(". Y:");
Serial.print(y);
Serial.print(". Z:");
Serial.println(z);


}


void Ball::colorFade (void){ // fades from previos color to the new one in 400 ms. (i.e. 80 samples) calles from isr.


}
