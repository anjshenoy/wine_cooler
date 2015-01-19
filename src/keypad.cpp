#include <Arduino.h>
#include "keypad.h"

KeyPad::KeyPad(int const* keyPins, int leftPin, int rightPin){
  _keyPins = keyPins;
  _leftPin = leftPin;
  _rightPin= rightPin;
  _prevState = 0;
  _lastTimeKeyPressed = 0;
}

void KeyPad::begin(){
  for(int i=0; i < 5; i++) {
    pinMode(_keyPins[i], INPUT);
  }

  pinMode(_leftPin, OUTPUT);
  pinMode(_rightPin, OUTPUT);
}

void KeyPad::setHandlers(KeyEventHandler *left, KeyEventHandler *right) {
  _leftKeyPadHandler = left;
  _rightKeyPadHandler = right;
 }

unsigned int KeyPad::readState(){
  unsigned int state = 0;
  int tmp;

  for(int leftSide = 0; leftSide < 2; leftSide++) {
    //select left side first
    digitalWrite(_leftPin, leftSide);
    digitalWrite(_rightPin, leftSide ^ 0x01);

    //go through 5 pins && read state for each
    for(int pin=0; pin < 5; pin++) {
      tmp = digitalRead(_keyPins[pin]) ^ 0x01;
      state <<= 1;
      state |= tmp;
    }
  }
  return state;
}

#define KEY_TO_MASK(key) (1 << key)
#define KEY_DOWN(state, key) ((state & KEY_TO_MASK(key)) != 0)
#define KEY_UP(state, key) ((state & KEY_TO_MASK(key)) == 0)

void KeyPad::checkForKeyPressed(){
  unsigned int state = readState();
  for(int key=0; key < 10; key++) {
    // going from up to down
    if(KEY_UP(_prevState, key) && KEY_DOWN(state, key)){
      _lastTimeKeyPressed = millis();
      switch(key) {
        case 0: _rightKeyPadHandler->onDownKeyPressed();
                break;
        case 1: _rightKeyPadHandler->onUpKeyPressed();
                break;
        case 2: _rightKeyPadHandler->onRightKeyPressed();
                break;
        case 3: _rightKeyPadHandler->onCenterKeyPressed();
                break;
        case 4: _rightKeyPadHandler->onLeftKeyPressed();
                break;
        case 5: _leftKeyPadHandler->onRightKeyPressed();
                break;
        case 6: _leftKeyPadHandler->onUpKeyPressed();
                break;
        case 7: _leftKeyPadHandler->onDownKeyPressed();
                break;
        case 8: _leftKeyPadHandler->onCenterKeyPressed();
                break;
        case 9: _leftKeyPadHandler->onLeftKeyPressed();
                break;
      }
    }
    else{
      long idleTime = millis() - _lastTimeKeyPressed;
      _rightKeyPadHandler->onIdle(idleTime);
      _leftKeyPadHandler->onIdle(idleTime);
    }
  }
  _prevState = state;
}

