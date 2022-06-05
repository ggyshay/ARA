#include <stdlib.h>
#include <functional>
#include "Arduino.h"
#define debounceDelay 200
class Button
{
public:
    bool *value;
    bool isReleaseSensitive;
    Button(bool *_value, bool _isReleaseSensitive)
    {
        value = _value;
        isReleaseSensitive = _isReleaseSensitive;
    }
    Button(std::function<void(void)> _callback)
    {
        callback = _callback;
        isControled = true;
    }
    void setPointer(bool *ptr)
    {
        value = ptr;
        isControled = false;
    }

    void setPointer(std::function<void(void)> _callback)
    {
        callback = _callback;
        isControled = true;
    }

    void unpress()
    {
        *value = false;
    }
    // std::function<void(void)> onToggle = nullptr;

    void setReading(bool reading)
    {

        if (((millis() - lastDebounceTime) > debounceDelay) && reading != buttonState)
        {
            if (isControled)
            {
                if (reading == HIGH)
                {
                    callback();
                }
            }
            else if (isReleaseSensitive)
            {
                *value = reading;
            }
            else
            {
                if (reading == HIGH)
                {
                    *value = !*value;
                    // if (onToggle != nullptr)
                    //     onToggle();
                }
            }
            buttonState = reading;
            lastDebounceTime = millis();
        }
    }

private:
    bool buttonState = false;
    bool isControled = false;
    unsigned long lastDebounceTime = 0;
    std::function<void(void)> callback;
};