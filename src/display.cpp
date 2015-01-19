#include "display.h"

byte Display::_lowerDigitAddress[] = {
  0xaf, 0x06, 0x6d, 0x4f, 0xc6, 0xcb, 0xeb, 0x0e, 0xef, 0xce
};

Display::Display(HT1621 &lcdDisplay, bool lowerZone, bool fahrenheit) : _lcdDisplay(lcdDisplay)
{
  _lowerZone = lowerZone;
  _fahrenheit = fahrenheit;
  _firstDigitAddress = _lowerZone ? 9 : 21;
  _secondDigitAddress = _lowerZone ? _firstDigitAddress + 2 : 19;
};

void Display::print(int number){
  int tensDigit = (number / 10) % 10;
  int unitsDigit = number % 10;

  if(tensDigit > 0){
    writeDigitToLCD(convert(tensDigit), _firstDigitAddress);
  } else {
    _lcdDisplay.memset(_firstDigitAddress, 0, 2);
  }
  writeDigitToLCD(convert(unitsDigit), _secondDigitAddress);
  writeTemperatureUnitToLCD();
};

void Display::writeDigitToLCD(byte digit, int address){
  _lcdDisplay.writeMem(address, digit >> 4);
  _lcdDisplay.writeMem(address + 1, digit);
};

void Display::writeTemperatureUnitToLCD(){
  writeDegree();
  if(_fahrenheit){
    _lowerZone ? displayLowerFahrenheit() : displayUpperFahrenheit();
  } else {
    //TODO: displayLower and upper celsius
  }
};

void Display::displayLowerCelsius(){};

void Display::displayUpperCelsius(){};

void Display::displayLowerFahrenheit(){
  _lcdDisplay.writeMem(13, 6);
  _lcdDisplay.writeMem(14, 12);
};

void Display::displayUpperFahrenheit(){
  _lcdDisplay.writeMem(15, 3);
  _lcdDisplay.writeMem(16, 6);
};

void Display::writeDegree(){
  _lowerZone ? _lcdDisplay.writeMem(13, 8) : _lcdDisplay.writeMem(16, 1);
};

void Display::setUnits(bool fahrenheit){
  _fahrenheit = fahrenheit;
};

byte Display::convert(int digit){
  byte value = _lowerDigitAddress[digit];
  return _lowerZone ? value : reverse(value);
};

byte Display::reverse(byte value){
  byte retval = 0;
  for(int i = 0; i < 8; i++){
     byte v = value & 1;
     retval = (retval << 1) | v;
     value = value >> 1;
  } 
  return retval;

};
