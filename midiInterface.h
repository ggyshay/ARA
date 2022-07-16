#include <stdlib.h>
#include <functional>
#include "midiEvent.h"
#include "arduino.h"

class MIDIInterface
{
private:
    std::function<void(void)> onClockCallback;
    byte counter = 0;
    IntervalTimer internalClockTimer;
    bool locked = true;

public:
    void begin();
    void setHandleClock(std::function<void(void)> callback);
    void sendMessage(EdgeEvent event);
    void update();
    void midiTick();
    void stopClock();
    void startClock();
    std::function<void(void)> onStopClock;
};

extern MIDIInterface midiInterface;