#include "outputBuffer.h"

bool bufferSorter(EdgeEvent a, EdgeEvent b)
{
    return a.timestamp < b.timestamp;
}

int OutputBuffer::registerEvents(MIDIEvent **(events))
{
    unsigned long now = micros();
    for (byte i = 0; i < 8; i++)
    {
        MIDIEvent *e = events[i];
        if (e != nullptr)
        {
            EdgeEvent risingEdge(now, e->channel, e->note, e->velocity);
            EdgeEvent fallingEdge(now + e->duration, e->channel, e->note, 0);
            buffer.push_back(risingEdge);
            buffer.push_back(fallingEdge);
        }
    }
    // Serial.println("");
    std::sort(buffer.begin(), buffer.end(), bufferSorter);
    return buffer.front().timestamp - now;
}

void OutputBuffer::setOutputCallback(std::function<void(EdgeEvent e)> callback)
{
    sendCallback = callback;
}

unsigned long OutputBuffer::sendNext()
{
    if (buffer.size() == 0)
    {
        return MAX_U_LONG_VAL;
    }
    unsigned long now = micros();
    while ((buffer.front().timestamp < (now + 1)) && (buffer.size() > 0))
    {
        sendCallback(buffer.front());
        buffer.erase(buffer.begin());
    }
    if (buffer.size() == 0)
    {
        return MAX_U_LONG_VAL;
    }
    unsigned long frontTS = buffer.front().timestamp;
    return frontTS - now;
}

void OutputBuffer::clear()
{
    buffer.clear();
}