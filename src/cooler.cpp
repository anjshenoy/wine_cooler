//module that runs the upper or lower unit
//does the following:
// 1. get current temperature
// 2. check current mode
// 3. if mode == normal display temperature else display set temperature
// 4. react to key input: if key is pressed, change from normal mode to input mode
// 5. if left key is pressed, decrease set temperature by 1 unit for each time pressed
// 6. if right is pressed, increase set temperature by 1 unit for each time pressed
// 7. if center is pressed, toggle light (on/off)
// 8. if mode != normal and last key pressed was 5 seconds ago, switch to normal mode and turn off light
// 9. calculate new peltier output based on current temperature and set temperature
// 10.repeat

#include "cooler.h"

CoolerImpl::CoolerImpl(Display &display, Thermometer &thermometer, int lightPin, int coolingPin):
  _display(display),
  _thermometer(thermometer),
  _normalState(this),
  _inputState(this) {
  _mode = NORMAL_MODE;
  _light = 0;
  _lightPin = lightPin;
  _coolingPin = coolingPin;
  _setPoint = 55;

  //assume we have a PWM range of 0-255
  // output = _offsetConstant + (_dampingConstant * error)
  // where error = _temperature - _setPoint
  _dampingConstant = 0.08;
  _offsetConstant = 0.5;
}

void CoolerImpl::begin() {
  pinMode(_lightPin, OUTPUT);
  pinMode(_coolingPin, OUTPUT);
  digitalWrite(_lightPin, _light);
}

void CoolerImpl::toggleLight() {
  _light ^= 0x01;
  digitalWrite(_lightPin, _light);
}

void CoolerImpl::adjustCoolingElement() {
  Serial.println("Adjusting cooling element. Weeeeeeeeeeeeee!");
}

void CoolerImpl::decreaseSetPoint() {
  _setPoint = _setPoint == MIN_SET_POINT ? MIN_SET_POINT : _setPoint - 1;
}

void CoolerImpl::increaseSetPoint() {
  _setPoint = _setPoint == MAX_SET_POINT ? MAX_SET_POINT : _setPoint + 1;
}

//Cooler lowerCooler(lowerDisplay, lowerThermometer);
//Cooler upperCooler(upperDisplay, upperThermometer);
//KeyPad keyPad( ..., lowerCooler, upperCooler);
//
//void setup() {
//  keyPad.setHandlers(&lowerCooler, &upperCooler);
//  lowerCooler.begin();
//  upperCooler.begin();
//
//}
//void loop() {
//  keypad.distributeEvent();
//  lowerCooler.onTimerEvent();
//  upperCooler.onTimerEvent();
//  delay(100);
//}

//void Cooler::run(){
//  if(isLightKeyPressed()) {
//    toggleLight();
//  }
//  if(isNormalMode()){
//    handleNormalMode();
//  } else {
//    handleInputMode();
//  }
//  _keyPad.readState()
//  _mode = _states[_mode].onTimerEvent();
//  adjustCoolingElement();
//}



//void Cooler::handleNormalMode(){
//  displayTemperature(_thermometer.temperature());
//  if(_keyPad.isLeftKeyPressed() || _keyPad.isRightKeyPressed()) {
//    _mode = INPUT_MODE;
//    _lastKeyPressTime = millis();
//  }
//}
//
//void Cooler::handleInputMode(){
//  if((millis() - _lastKeyPressTime) > 5000) {
//    _mode = NORMAL_MODE;
//  } else {
//    _setPoint = readSetPoint();
//    _lastKeyPressTime = millis();
//    if(_keyPad.isLeftKeyPressed()){
//      _setPoint = _setPoint == MIN_SET_POINT ? MIN_SET_POINT : _setPoint - 1;
//    } else {
//      _setPoint = _setPoint == MAX_SET_POINT ? MAX_SET_POINT : _setPoint + 1;
//    }
//  }
//  displayTemprature(setPoint);
//}

//void Cooler::onCenterKeyPressed() {
//  toggleLight();
//}
//
//void Cooler::onLeftKeyPressed() {
//  lastKeyPressTime = millis();
//
//  if(isNormalMode()) {
//    _mode = INPUT_MODE;
//  }
//  else {
//  }
//}
//
//void Cooler::onRightKeyPressed() {
//  lastKeyPressTime = millis();
//
//  if(isNormalMode()) {
//    _mode = INPUT_MODE;
//  }
//  else {
//  }
//}
//
//
//bool Cooler::isLightKeyPressed(){
//  int state = _keyPad.readState();
//  _upper ? keyPressed(KEY_RIGHT_CENTER, state) : keyPressed(KEY_LEFT_CENTER, state);
//}
//
//bool Cooler::isLeftKeyPressed(){
//  int state = _keyPad.readState();
//  _upper ? keyPressed(KEY_RIGHT_LEFT, state) : keyPressed(KEY_LEFT_LEFT, state);
//}
//
//bool Cooler::isRightKeyPressed(){
//  int state = _keyPad.readState();
//  _upper ? keyPressed(KEY_RIGHT_RIGHT, state) : keyPressed(KEY_LEFT_RIGHT, state);
//}
