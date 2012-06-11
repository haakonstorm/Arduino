#include <Ball2.h>
#include <MsTimer2.h>
#include <EEPROM.h>


// Lag en instans av klassen ball.
Ball2 ball;


// Setup er en standardfunksjon i arduino. Når programmet kjøres vil det alltid starte her. Legg merke til at det første vi gjør er å sette opp 
// analog referance for AD-konverter og endre timere for PWM til styring av lysdioder. Det vill kanskje vært naturlig å gjøre dette i 
// Konstruktoren til klassen Ball, men av en eller annen grunn er det ikke tillatt av Arduino. Derfor setter vi de opp her.

void setup(){
        // The voltage applied to the AREF pin (0 to 5V only) is used as the reference.
	analogReference(EXTERNAL);
 
	// Adjust Arduino PWM frequencies. See http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1235060559
	// Pins 5 and 6  are controlled by 8-bit Timer/Counter0 in fast PWM mode.
	// This affects millis() and delay().
	// Frequency = 16000000 / (256 * N) = 62500 / N
	// 0x01     1  62500
	// 0x02     8  7812,5
	// 0x03    64  976,5625 (default)
	// 0x04   256  244,140625
	// 0x05  1024  61,03515625 
	TCCR0B = TCCR0B & 0b11111000 | 0x02;
			
	// Pins 9 and 10  are controlled by 16-bit Timer/Counter1 in phase correct PWM mode.
	// Frequency = 16000000 / (510 * N) = 31372,549019608 / N
	// 0x01     1  31372,549019608
	// 0x02     8  3921,568627451
	// 0x03    64  490,196078431 (default)
	// 0x04   256  122,549019608
	// 0x05  1024  30,637254902	 	
	TCCR1B = TCCR1B & 0b11111000 | 0x02;

  
        // Sett opp seriporten til å kommunisere på 19200 Baud. 1 stoppbit, 1 startbit, ingen paritet.
        Serial.begin(19200);
  
        // MsTimer2 er en klasse som pakker inn timerinterupt på timer 2. Vi setter den opp med to verdier: 
        // 1: antall millisekunder mellom hver gang timeren gir interrupt.
        // 2: navnet på funksjonen som skal kalles, i vårt tilfelle, detectNullG().  
        MsTimer2::set(5, detectNullG); // 5ms period, 200Hz
        
        // start timer 2.
        MsTimer2::start();
        // sett på litt lys når ballen er i gang. Ellers er det ikke lett å se at den er på, og vi lader ut batteriet for det vi ikke skrur den av.
        ball.setColor(3,0,3);
}


// Funksjon som kalles fra timer 2 interrupt.
void detectNullG(){ 
 
  // vi kaller funksjonen som leser sensorer og regner ut kraften på ballen.
    ball.processAD();
  // HVis ballen er i lufta skal den være rød
  if(ball.getInAir()){
        ball.setColor(255, 0, 0);
  }else{  // Ellers lyse svakt.
    ball.setColor(3,0,3);
  }
  
  

}


// Loop er en funksjon som kreves av Arduino. Etter at setup er ferdig vil programmet kjøre innholdet i loop kontinuerlig. Sinde vi bruker timer interrupt er denne funksjonen tom.
void loop(){
 
}
  
  
  
  

  

  
  
  
  

