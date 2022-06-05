#include "Interface.h"
#include "sequencerEngine.h"
#include "Scheduler.h"
#include "midiInterface.h"
#include "midiEvent.h"
#include "outputBuffer.h"

Interface interface;
SequencerEngine sequencer;
Scheduler scheduler;
MIDIInterface midiInterface;
OutputBuffer outputBuffer;
void triggerSend()
{
    unsigned long timeToNextEventMicros = outputBuffer.sendNext();
    scheduler.registerTimeToNextEvent(timeToNextEventMicros);
}
void setup()
{
    Serial.begin(115200);
    delay(400);
    sequencer.setup();

    interface.currentInstrument = &(sequencer.currentInstrument);
    interface.currentInstrumentIndex = &sequencer.currentInstrumentIndex;
    interface.instruments = sequencer.instruments;
    interface.sendMessage = [](byte msg, byte value)
    {
        Serial.printf("send message: %d, %d\n", msg, value);
        sequencer.handleInterfaceMessage(msg, value);
    };
    interface.setup();
    Serial.println("interface setup");

    midiInterface.begin();
    midiInterface.setHandleClock([]() -> void
                                 {
        scheduler.onMIDIClock();
        MIDIEvent **event_ptr = sequencer.getNextEvent();
        
        outputBuffer.registerEvents(event_ptr);
        unsigned long timeToNextEvent = outputBuffer.sendNext();
        scheduler.registerTimeToNextEvent(timeToNextEvent); });
    midiInterface.onStopClock = []() -> void
    {
        sequencer.resetSequencerPosition();
    };
    Serial.println("midi interface setup");

    outputBuffer.setOutputCallback([](EdgeEvent e) -> void
                                   { midiInterface.sendMessage(e); });
    sequencer.bpmEstimate = &(scheduler.bpmEstimate);
    scheduler.setTickCallback(triggerSend);
    scheduler.begin();

    Serial.println("all setup");
};

void loop()
{
    if (!sequencer.nextEventReady)
    {
        sequencer.calculateNextEvent();
    }
    interface.update();
}