Smarte sjongleringsballer
=========================

Programmering av baller fra Macbook Pro Retina med OSX 10.8.2
-------------------------------------------------------------

### Setup
1. Last ned nyeste [Arduino.app][1]
2. Last ned [FTDI-drivere][2]
3. Start Arduino 
4. Tools >> Board >> Arduino Duemilanove med ATmega328 (Uno funker også!)
5. Tools >> Serial Port >> /dev/tty.* (noe med USB)
6. Tools >> Programmer >> USBtinyISP

### Burn bootloader
1. Hook opp AVR Pocket Programmer på USB
2. Koble til ballen slik at flatkabelen på ISP-headeren går “ut” fra kortet. [   X ] ===
3. Sett bryteren til POWER TARGET på Pocket Programmer
4. Skru av ballen (bryteren til høyre)
5. Tools >> BURN BOOTLOADER

### Upload firmware
1. File >> Compile & Upload

[1]: http://arduino.cc/en/Main/Software
[2]: http://www.ftdichip.com/Drivers/VCP.htm
