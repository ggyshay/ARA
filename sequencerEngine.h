#include <stdlib.h>
#include <functional>
#include "midiEvent.h"
#include "arduino.h"
#include "Constants.h"
#include "Note.h"
#include "Instrument.h"

#ifndef SEQUENCER_ENGINE_H
#define SEQUENCER_ENGINE_H
class SequencerEngine
{
public:
    void calculateNextEvent();
    void setup();
    bool nextEventReady;
    MIDIEvent **getNextEvent();
    float *bpmEstimate;
    void resetSequencerPosition();

    Instrument instruments[8];
    Instrument *currentInstrument = instruments;
    byte currentInstrumentIndex = 0;
    byte generalNoteIndex = 0;
    void handleInterfaceMessage(byte t, byte v);
    std::function<void(byte)> onError;

private:
    MIDIEvent *nextEvents[8];

    byte lastCalculatedNote = 0;

    String toString();
};
#endif
