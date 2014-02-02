jy-mcu-3208-Arduino-Library
===========================

This library is a work in progress.  It is a collection of all the hardware on the JY-MCU 3208 Clock module available from your favourite chinese emporiums.

The library contains several classes, one for the HT1632C led driver, one for the DS1302 clock module.  

Code is borrowed from where ever I can find it, I will build in credits when I get time.  If you look in the library files at the moment the original comments are still there.

I have where possible optimised the existing libraries to bring the compiled sizes as low as possible, the AtMega8 micro-controllers have around 8k of memory which is a fair bit less than your normal arduino.

Before you can use this library, you will probably need to set the fuses on your AtMega chip.   I will include upload a bootloader that does this as soon as I can.
