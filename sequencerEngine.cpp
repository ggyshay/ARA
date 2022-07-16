#include "midiEvent.h"
#include "sequencerEngine.h"

void SequencerEngine::setup()
{
    for (int i = 0; i < 8; i++)
    {
        instruments[i].pitch = 36 + i;
    }
}

void SequencerEngine::calculateNextEvent()
{
    // Note curr_note = currentInstrument->notes[lastCalculatedNote];
    // byte velocity = curr_note.gate ? curr_note.velocity : 0;
    // float bpm = *bpmEstimate;
    // int duration = curr_note.duration * 150000.0 / bpm;
    // Serial.printf("bpm estimate: %f, duration: %d\n", bpm, duration);
    // nextEvent.set(0, 69, velocity, duration);
    // nextEvent.set(0, curr_note.pitch, velocity, duration);
    for (byte i = 0; i < 8; i++)
    {
        nextEvents[i] = instruments[i].calculateNextEvent(*bpmEstimate);
    }
    nextEventReady = true;
    // lastCalculatedNote = (lastCalculatedNote + 1) % currentInstrument->sequenceLength;
}

MIDIEvent **SequencerEngine::getNextEvent()
{
    if (nextEventReady == false)
    {
        Serial.println("tryied sending none event");
    }
    for (byte i = 0; i < 8; i++)
    {
        instruments[i].getNextEvent();
    }
    generalNoteIndex = (generalNoteIndex + 1) % 64;
    nextEventReady = false;
    return nextEvents;
}

void SequencerEngine::resetSequencerPosition()
{
    nextEventReady = false;
    lastCalculatedNote = 0;
    generalNoteIndex = 0;
    for (byte i = 0; i < 8; i++)
    {
        instruments[i].resetSequencePosition();
    }
}

void SequencerEngine::handleInterfaceMessage(byte t, byte v)
{
    switch (t)
    {
    case MSG_CHANGE_INSTRUMENT:
    {

        currentInstrument = instruments + v;
        Serial.printf("currentInstrument Engine: %d\n", currentInstrument);
        currentInstrumentIndex = v;
        break;
    }
    case MSG_CHANGE_LENGTH:
    {

        currentInstrument->sequenceLength = v;
        currentInstrument->currentNote = generalNoteIndex % v;
        break;
    }
    case MSG_CLEAR_ALL:
    {
        for (byte i = 0; i < 8; i++)
        {
            instruments[i].reset(generalNoteIndex);
        }
        break;
    }
    case MSG_CLEAR_INSTRUMENT:
        instruments[v].reset(generalNoteIndex);
        break;
    case MSG_TOGGLE_MUTE_INSTRUMENT:
        instruments[v].isMuted = !instruments[v].isMuted;
        break;

    case MSG_DUPLICATE_LOOP:
    {

        currentInstrument->duplicateLoop();
    }
    }
}
