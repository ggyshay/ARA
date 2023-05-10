#include <stdlib.h>
#include <functional>
#include "constants.h"

#include "midiInterface.h"

static void handleMIDIClockOuter()
{
    // Serial.println("handle clock outer");
    midiInterface.midiTick();
}
static void handleMIDIClockStopOuter()
{
    // Serial.println("stop clock outer");
    midiInterface.stopClock();
}

static void handleMIDIClockStartOuter()
{
    // Serial.println("start clock outer");
    midiInterface.startClock();
}

static void handleScheduleMIDIReadTick()
{
    midiInterface.update();
}

void MIDIInterface::begin()
{
    usbMIDI.setHandleClock(handleMIDIClockOuter);
    usbMIDI.setHandleStop(handleMIDIClockStopOuter);
    usbMIDI.setHandleStart(handleMIDIClockStartOuter);
    internalClockTimer.begin(handleScheduleMIDIReadTick, 250);
}

void MIDIInterface::midiTick()
{
    if (locked)
    {
        onError(ERR_LOCK);
        return;
    }
    if (counter == 0)
    {
        onClockCallback();
    }
    counter = (counter + 1) % 6;
}

void MIDIInterface::stopClock()
{
    counter = 0;
    locked = true;
    onStopClock();
    onError(ERR_NO_ERROR);
}

void MIDIInterface::startClock()
{
    counter = 0;
    locked = false;
    onError(ERR_NO_ERROR);
}

void MIDIInterface::setHandleClock(std::function<void(void)> callback)
{
    onClockCallback = callback;
}

void MIDIInterface::sendMessage(EdgeEvent event)
{
    usbMIDI.sendNoteOn(event.note, event.velocity, event.channel);
    usbMIDI.send_now();
}

void MIDIInterface::update()
{
    while (usbMIDI.read())
        ;
}