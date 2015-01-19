#include "thermometer.h"
#include <Arduino.h>

#define sign(x) ((x > 0) ? 1 : ((x < 0) ? -1 : 0))

Thermometer::Thermometer(DallasTemperature &sensor, const uint8_t * const address):
  _sensor(sensor)
{
  _address = address;
}

void Thermometer::begin() {
  _sensor.setResolution(_address, 9);
  _lastReadTime = 0;
  // magic number to indicate _temperature does not have a real value
  _temperature = -4242;
  getTemperature();
}

float Thermometer::getTemperature() {
  if((millis() - _lastReadTime) > READ_WAIT_TIME) {
    //Serial.print("getting temperature: ");
    _sensor.requestTemperaturesByAddress(_address);

    float currentTemperature = _sensor.getTempF(_address);

    if((currentTemperature > 40) && (currentTemperature < 90)) {
      if((_temperature > 40) && (abs(_temperature - currentTemperature) > 0.5) ) {
        _temperature += sign(currentTemperature - _temperature) * 0.5;
      }
      else {
        _temperature = currentTemperature;
      }
    }

    Serial.println(_temperature);
    _lastReadTime = millis();
  }

  return _temperature;
}
