#include "Arduino.h"
#include "constants.h"
#define S0 2
#define S1 14
#define S2 7
#define S3 8

#define RED_LED 15
#define GREEN_LED 22
#define BLUE_LED 23

#define INSTRUMENT_LED_RED 11
#define INSTRUMENT_LED_GREEN 12
#define INSTRUMENT_LED_BLUE 28

#define PAGE_LED 9

#define INSTRUMENT_BUTTON 26
#define STEP_PIN 10
#define POTS_PIN 31
#define CONTROL_BUTTON 24

void setupPins();

void selectChannel(byte channel);

bool getSteps();
bool getInstButtons();
bool getControlButtons();

byte getPotValue();

void writeInstLed(byte color);
void writePageLed(bool value);
void writeStepLed(byte color);
