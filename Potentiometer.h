#include "arduino.h"
#define POT_RESOLUTION 2
#define POT_TIMEOUT 300

class Potentiometer
{
public:
    Potentiometer(std::function<void(byte value)> _valueSetter)

    {
        valueSetter = _valueSetter;
        _value = 0;
    }

    bool setReading(byte value, unsigned long now)
    {
        if (_value - value > POT_RESOLUTION || _value - value < -POT_RESOLUTION)
        {
            potTime = now;
        }

        if (potTime + POT_TIMEOUT > now)
        {
            moving = true;
        }
        else
        {
            moving = false;
        }

        if (moving)
        {
            _value = value;
            valueSetter(value / 2);
            return true;
        }
        return false;
    }
    unsigned long potTime;
    bool moving;
    std::function<void(byte value)> valueSetter;
    byte _value;
};
