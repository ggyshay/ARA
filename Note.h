#ifndef NOTE_H
#define NOTE_H
#include "Constants.h"

class Note
{
public:
    bool velocity = 1;
    bool gate = false;
    void reset()
    {
        velocity = 1;
        gate = false;
    }
    void toggle()
    {
        gate = !gate;
    }
    void copy(Note other)
    {
        velocity = other.velocity;
        gate = other.gate;
    }
    void toggleAcc()
    {
        velocity = !velocity;
    }

    // String toString()
    // {
    //     String res = "";
    //     res += noteName[pitch % 12];
    //     res += (pitch / 12 - 3);
    //     // res += " ";
    //     return res;
    // }
};
#endif