#ifndef H_INSTRUMENT
#define H_INSTRUMENT
#include "Arduino.h"
#include "Note.h"
#include "midiEvent.h"

class Instrument
{
public:
    byte sequenceLength = 16;
    byte currentNote = 0;
    bool isMuted = false;
    Note notes[64];
    byte pitch = 63;
    byte velocity = 127;
    bool repeating = false;
    void reset()
    {
        currentNote = 0;
        sequenceLength = 16;
        for (byte i = 0; i < 64; i++)
        {
            notes[i].reset();
        }
        isMuted = false;
    }
    void resetSequencePosition()
    {
        currentNote = 0;
    }

    void getNextEvent()
    {
        currentNote = (currentNote + 1) % sequenceLength;
        // nextEventReady = false;
        // return &nextEvent
    }

    void duplicateLoop()
    {
        byte oldSequenceLength = sequenceLength;
        sequenceLength *= 2;
        if (sequenceLength > 64)
        {
            sequenceLength = 64;
        }
        for (byte i = 0; i < oldSequenceLength; i++)
        {
            if (i + oldSequenceLength >= 64)
            {
                continue;
            }
            notes[i + oldSequenceLength].copy(notes[i]);
        }
    }

    MIDIEvent *calculateNextEvent(float bpm)
    {
        if (!repeating && (isMuted || (notes[currentNote].gate == 0)))
        {
            return nullptr;
        }
        float noteVelocity = 1.0 * (notes[currentNote].velocity * 37 + 90);
        noteVelocity *= velocity / 127.0;
        nextEvent.set(0, pitch, (byte)noteVelocity, 80 * 150000.0 / bpm);

        return &nextEvent;
        // nextEventReady = true;
    }

private:
    MIDIEvent nextEvent;
    bool nextEventReady = false;
};
#endif