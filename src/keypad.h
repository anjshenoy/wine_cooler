#ifndef KEYPAD_H
#define KEYPAD_H

#include <Arduino.h>

#define KEY_LEFT_LEFT    0b1000000000
#define KEY_LEFT_CENTER  0b0100000000
#define KEY_LEFT_DOWN    0b0010000000
#define KEY_LEFT_UP      0b0001000000
#define KEY_LEFT_RIGHT   0b0000100000
#define KEY_RIGHT_LEFT   0b0000010000
#define KEY_RIGHT_CENTER 0b0000001000
#define KEY_RIGHT_RIGHT  0b0000000100
#define KEY_RIGHT_UP     0b0000000010
#define KEY_RIGHT_DOWN   0b0000000001

#define keyPressed(key, state) (key & state)

class KeyEventHandler {
  public:
    virtual void onRightKeyPressed() = 0;
    virtual void onLeftKeyPressed() = 0;
    virtual void onCenterKeyPressed() = 0;
    virtual void onUpKeyPressed() = 0;
    virtual void onDownKeyPressed() = 0;
    virtual void onIdle(long idleTime) = 0;
};

class KeyPad{
  public:
    //keypins are in the order:
    // right side: left, center, right, up, down
    // left side : left, center, down, up, right
    KeyPad(int const* keyPins, int leftPin, int rightPin);
    void begin();
    void setHandlers(KeyEventHandler *left, KeyEventHandler *right);
    void checkForKeyPressed();

  private:
     unsigned int readState();

     int const* _keyPins;
     int _leftPin;
     int _rightPin;

     int _prevState;
     long _lastTimeKeyPressed;
     KeyEventHandler *_leftKeyPadHandler;
     KeyEventHandler *_rightKeyPadHandler;
};

#endif
