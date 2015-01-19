#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <HT1621.h>

class Display{

  public:
    Display(HT1621 &lcdDisplay, bool lowerZone = true, bool fahrenheit = true);
    void print(int number);
    void setUnits(bool fahrenheit = true);

  private:
    bool _lowerZone;
    bool _fahrenheit;
    int _firstDigitAddress;
    int _secondDigitAddress;
    HT1621 &_lcdDisplay;

    static byte _lowerDigitAddress[];

    byte convert(int digit);
    byte reverse(byte value);
    void writeDigitToLCD(byte digit, int address);
    void writeTemperatureUnitToLCD();
    void writeDegree();
    void displayLowerFahrenheit();
    void displayUpperFahrenheit();
    void displayLowerCelsius();
    void displayUpperCelsius();
};

#endif
