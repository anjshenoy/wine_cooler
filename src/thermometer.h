#ifndef THERMOMETER_H
#define THERMOMETER_H

#include <DallasTemperature.h>

#define READ_WAIT_TIME 2000

class Thermometer {
  public:
    Thermometer(DallasTemperature &sensor, const uint8_t * const address);
    void begin();
    float getTemperature();

  private:
    float _temperature;
    long _lastReadTime;
    const uint8_t * _address;
    DallasTemperature &_sensor;
};


#endif
