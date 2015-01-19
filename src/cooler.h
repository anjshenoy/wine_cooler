#ifndef COOLER_H
#define COOLER_H

#include <Arduino.h>
#include "thermometer.h"
#include "display.h"
#include "keypad.h"

#define NORMAL_MODE 0
#define INPUT_MODE 1
#define MIN_SET_POINT 50
#define MAX_SET_POINT 66

#define MAX_ANALOG_VALUE 255

class Cooler : public KeyEventHandler {
  public:
    virtual void toggleLight() = 0;
    virtual void displayTemperature(int temp) = 0;
    virtual void increaseSetPoint() = 0;
    virtual void decreaseSetPoint() = 0;
    virtual int getSetPoint() = 0;
    virtual int getTemperature() = 0;
};

class CoolerState {
  public:
    CoolerState(Cooler * const cooler) : _cooler(cooler) {};
    virtual int onRightKeyPressed() = 0;
    virtual int onLeftKeyPressed() = 0;
    virtual int onTimerEvent() = 0;
    virtual int onIdle(long idleTime) = 0;

  protected:
    Cooler * const _cooler;
};

class NormalState : public CoolerState {
  public:
    NormalState(Cooler * const cooler) : CoolerState(cooler) {};

    int onRightKeyPressed() { return INPUT_MODE; };

    int onLeftKeyPressed() { return INPUT_MODE; };

    int onTimerEvent() {
      _cooler->displayTemperature(_cooler->getTemperature());
      return NORMAL_MODE;
    };

    int onIdle(long idleTime) { return NORMAL_MODE; };
};

class InputState : public CoolerState {
  public:
    InputState(Cooler * const cooler) : CoolerState(cooler) { };

    int onRightKeyPressed() {
      _cooler->increaseSetPoint();
      return INPUT_MODE;
    };

    int onLeftKeyPressed() {
      _cooler->decreaseSetPoint();
      return INPUT_MODE;
    };

    int onTimerEvent() {
        _cooler->displayTemperature(_cooler->getSetPoint());
        return INPUT_MODE;
    };

    int onIdle(long idleTime) {
      if(idleTime > 5000) {
        return NORMAL_MODE;
      }
      else {
        return INPUT_MODE;
      }
    }
};

class CoolerImpl : public Cooler {
  public:
    CoolerImpl(Display &display, Thermometer &thermometer, int lightPin, int coolingPin);

    void begin();

    void toggleLight();

    void displayTemperature(int temp) { _display.print(temp); };

    void increaseSetPoint();

    void decreaseSetPoint();

    void adjustCoolingElement();

    int getSetPoint()  { return _setPoint; };

    int getTemperature() {
      return _thermometer.getTemperature() + 0.5;
    };

    void setCooling() {
      float error = _thermometer.getTemperature() - _setPoint;
      float output = _offsetConstant + (_dampingConstant * error);
      if (output > 1.0) {
        output = 1.0;
      }
      else if (output < 0.0) {
        output = 0.0;
      }
      //Serial.print("Output = ");
      //Serial.println(output);
      //Serial.print("Set Value = ");
      int sv = output * MAX_ANALOG_VALUE + 0.5;
      //Serial.println(sv);

      //analogWrite(_coolingPin, 128);
      analogWrite(_coolingPin, sv);
      //analogWrite(_coolingPin, output * MAX_ANALOG_VALUE + 0.5);
    }

    void onTimerEvent() {
      setCooling();
      _mode = getState().onTimerEvent(); 
    };

    CoolerState & getState() { 
      if(_mode == INPUT_MODE) {
        return _inputState;
      }
      else {
        return _normalState;
      }
    };

    // KeyEventHandler methods for states
    void onRightKeyPressed() { 
      _mode = getState().onRightKeyPressed();
    };

    void onLeftKeyPressed() {
      _mode = getState().onLeftKeyPressed();
    };

    void onIdle(long idleTime) {
      _mode = getState().onIdle(idleTime);
    }

    // Other KeyEventHandler methods
    void onCenterKeyPressed() { toggleLight(); };

    void onUpKeyPressed() { };

    void onDownKeyPressed() { };

  private:
    Display &_display;
    Thermometer &_thermometer;
    NormalState _normalState;
    InputState _inputState;
    int _mode;
    int _setPoint;
    int _light;
    int _lightPin;
    int _coolingPin;
    float _dampingConstant;
    float _offsetConstant;

};

#endif
