#include <EEPROM.h>
#include <Ball.h>
#include <MsTimer2.h>


void setup(){
//	 analogReference(EXTERNAL);

  	//øk frekvense på pwm: se http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1235060559
    TCCR0B = TCCR0B & 0b11111000 | 0x02;
    TCCR1B = TCCR1B & 0b11111000 | 0x02;
    
  	pinMode(BLUE, OUTPUT);
  	pinMode(RED, OUTPUT);
  	pinMode(GREEN, OUTPUT);
  	Serial.begin(19200);

  	xN = (EEPROM.read(xEepromHigh) * 256) + EEPROM.read(xEepromLow);
  	yN = (EEPROM.read(yEepromHigh) * 256) + EEPROM.read(yEepromLow);
  	zN = (EEPROM.read(zEepromHigh) * 256) + EEPROM.read(zEepromLow);
  
  	MsTimer2::set(5, sampleAD); // 5ms period, fSample = 200Hz. (too fast?)
  	MsTimer2::start();

}

void sampleAD (void){
  	static int x,y,z;
  	static int sum;
  	static int count = 0;

    x = abs(analogRead(X) - xN);
    y = abs(analogRead(Y) - yN);
    z = abs(analogRead(Z) - zN);

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

}


void colorFade (void){ // fades from previos color to the new one in 400 ms. (i.e. 80 samples) calles from isr.


}
void loop(){
  
}
