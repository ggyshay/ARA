#include "HAL.h"

void setupPins()
{
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(STEP_PIN, INPUT);
    pinMode(INSTRUMENT_LED_BLUE, OUTPUT);
    pinMode(INSTRUMENT_LED_GREEN, OUTPUT);
    pinMode(INSTRUMENT_LED_RED, OUTPUT);
    pinMode(INSTRUMENT_BUTTON, INPUT);
    pinMode(CONTROL_BUTTON, INPUT);
    pinMode(PAGE_LED, OUTPUT);
    analogReadResolution(10);
}

void selectChannel(byte channel)
{
    GPIOD_PDOR = (GPIOD_PDOR & 0xFFF0) | channel;
}

bool getSteps()
{
    return digitalReadFast(STEP_PIN);
}
bool getInstButtons()
{
    return digitalReadFast(INSTRUMENT_BUTTON);
}

bool getControlButtons()
{
    return digitalReadFast(CONTROL_BUTTON);
}

byte getPotValue()
{
    return 255 - analogRead(POTS_PIN) / 4;
}

void writeInstLed(byte color)
{
    digitalWriteFast(INSTRUMENT_LED_RED, color & C_RED);
    digitalWriteFast(INSTRUMENT_LED_GREEN, color & C_GREEN);
    digitalWriteFast(INSTRUMENT_LED_BLUE, color & C_BLUE);
}

void writePageLed(bool value)
{
    digitalWriteFast(PAGE_LED, value);
}

void writeStepLed(byte color)
{
    digitalWriteFast(RED_LED, color & C_RED);
    digitalWriteFast(GREEN_LED, color & C_GREEN);
    digitalWriteFast(BLUE_LED, color & C_BLUE);
}