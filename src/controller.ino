#include <OneWire.h>
#include <DallasTemperature.h>

#include <HT1621.h>
#include "display.h"
#include "keypad.h"
#include "cooler.h"
#include "thermometer.h"

#define DATA_KEY_PIN 4
#define WRITE_KEY_PIN 7
#define CS_KEY_PIN 8
#define UPPER_LIGHT_PIN 9
#define LOWER_LIGHT_PIN 10

// 13,6 are pins connected to Timer 4 (TCCR4B)
#define UPPER_COOLER_PIN 13
#define LOWER_COOLER_PIN 6


#define keyPressed(key, state) (key & state)
unsigned int keyState();

int keyPins[] = { A4, A3, A5, A1, A2};
#define RIGHT_PIN DATA_KEY_PIN
#define LEFT_PIN A0
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS_PIN 2


HT1621 lcdDisplay(DATA_KEY_PIN, WRITE_KEY_PIN, 1, CS_KEY_PIN);
KeyPad keyPad(keyPins, LEFT_PIN, RIGHT_PIN);

Display upperDisplay(lcdDisplay, false, true);
Display lowerDisplay(lcdDisplay, true, true);

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS_PIN);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensor(&oneWire);

//thermometer device addresses
uint8_t UPPER_THERMOMETER_ADDRESS[] = { 0x28, 0x00, 0xC8, 0xAB, 0x05, 0x00, 0x00, 0x0A };
uint8_t LOWER_THERMOMETER_ADDRESS[] = { 0x28, 0x51, 0x5C, 0xAB, 0x05, 0x00, 0x00, 0xBB };

Thermometer upperThermometer(sensor, UPPER_THERMOMETER_ADDRESS);
Thermometer lowerThermometer(sensor, LOWER_THERMOMETER_ADDRESS);
CoolerImpl upperCooler(upperDisplay, upperThermometer, UPPER_LIGHT_PIN, UPPER_COOLER_PIN);
CoolerImpl lowerCooler(lowerDisplay, lowerThermometer, LOWER_LIGHT_PIN, LOWER_COOLER_PIN);

void lcdDisplayPostBegin(){
  lcdDisplay.sendCommand(HT1621::SYS_EN);
  lcdDisplay.sendCommand(HT1621::ON_RC);
  lcdDisplay.sendCommand(HT1621::BIAS_HALF_4_COM);
  lcdDisplay.sendCommand(HT1621::LCD_ON);
  lcdDisplay.memset(0,0, 128);
}

void setup(){
  //Set the timer prescaler to 1 i.e. CS40 ==> 62.5kHz
  TCCR4B = _BV(CS40);
  TCCR4D = 0;

  //pinMode(11, OUTPUT);
  //digitalWrite(11, HIGH);
  //set PWM to fast Pulse width modulation
  Serial.begin(115200);
  lcdDisplay.begin();
  lcdDisplayPostBegin();

  sensor.begin();
  upperThermometer.begin();
  lowerThermometer.begin();

  upperCooler.begin();
  lowerCooler.begin();


  keyPad.setHandlers(&upperCooler, &lowerCooler);
  keyPad.begin();
}

void loop(){
  //blink(1, 100);

  keyPad.checkForKeyPressed();

  upperCooler.onTimerEvent();
  lowerCooler.onTimerEvent();

  delay(250);
}

void keyBegin() {
  for(int i=0; i < 5; i++) {
    pinMode(keyPins[i], INPUT);
  }

  pinMode(LEFT_PIN, OUTPUT);
  pinMode(RIGHT_PIN, OUTPUT);
}

unsigned int keyState() {
  unsigned int state = 0;
  int tmp;

  for(int left = 0; left < 2; left++) {
    //select left side first
    digitalWrite(LEFT_PIN, left);
    digitalWrite(RIGHT_PIN, left ^ 0x01);

    //go through 5 pins && read state for each
    for(int key=0; key < 5; key++) {
      tmp = digitalRead(keyPins[key]) ^ 0x01;
      state <<= 1;
      state |= tmp;
    }
  }
  return state;
}

//unsigned int keyState1() {
//  unsigned int state = 0;
//  int key;
//
//  digitalWrite(LEFT_PIN, HIGH);
//  digitalWrite(RIGHT_PIN, LOW);
//
//  key = digitalRead(KEY_PIN_0) ^ 0x01;
//  state <<= 1;
//  state |= key;
//
//  key = digitalRead(KEY_PIN_1) ^ 0x01;
//  state <<= 1;
//  state |= key;
//
//  key = digitalRead(KEY_PIN_2) ^ 0x01;
//  state <<= 1;
//  state |= key;
//
//  digitalWrite(RIGHT_PIN, HIGH);
//  digitalWrite(LEFT_PIN, LOW);
//
//  key = digitalRead(KEY_PIN_0) ^ 0x01;
//  state <<= 1;
//  state |= key;
//
//  key = digitalRead(KEY_PIN_1) ^ 0x01;
//  state <<= 1;
//  state |= key;
//
//  digitalWrite(LEFT_PIN, LOW);
//  key = digitalRead(KEY_PIN_2) ^ 0x01;
//  state <<= 1;
//  state |= key;
//
//  digitalWrite(RIGHT_PIN, LOW);
//
//  return state;
//}

void blink(int times, int period) {
  for(int i=0; i < times; i++) {
     digitalWrite(13, LOW); 
     delay(period);
     digitalWrite(13, HIGH); 
     delay(period);
  }
}

//int probe(int duration){
//  int currentState = digitalRead(KEY_PIN);
//  int value;
//  int edgeCount = 0;
//
//  unsigned long endTime = millis() + duration;
//
//  while(millis() < endTime){
//    value = digitalRead(KEY_PIN);
//
//    //state change
//    if(currentState != value){
//      edgeCount ++;
//      currentState = value;
//    }
//  }
//  Serial.println(edgeCount + 1000);
//  return (edgeCount > 2 ? 2 : currentState);
//
//}
