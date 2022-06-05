
#include "Arduino.h"
#include "Button.h"
#include "Potentiometer.h"
#include "Instrument.h"

#include <stdlib.h>
#include <functional>
#include "Note.h"
#include "HAL.h"

#define NORMAL_MODE 0
#define LENGTH_MODE 1
#define ACC_MODE 2

class Interface
{
private:
    Button *stepButtons[16];
    Button *controllButtons[8];
    Button *instButtons[8];
    Potentiometer *instPots[8];
    bool tempControlButtonState[8];
    bool instButtonState[8];
    bool altStepButtonState[16];
    byte getStepColor(bool value, bool stepping);
    byte getInstColor(bool value);
    void detectInstrumentChange(byte i);
    byte stepMode = NORMAL_MODE;
    void setStepsPointersMode(byte mode);
    void detectCommand();
    void writeLedMode(byte i);
    void writeInstrumentLed(byte i);
    void unpressControlButtons();
    void detectPageChange();
    byte updateCounter = 0;
    byte currentPage = 0;

public:
    Instrument **currentInstrument;
    Instrument *instruments;
    byte *currentInstrumentIndex;
    // byte *currentStep;
    // Note *notes;
    void setup();
    void update();
    std::function<void(byte msg, byte value)> sendMessage;
};