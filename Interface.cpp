#include "Interface.h"

void Interface::setStepsPointersMode(byte mode)
{
    if (mode == stepMode)
    {
        return;
    }
    stepMode = mode;

    switch (mode)
    {
    case NORMAL_MODE:
    {

        for (byte i = 0; i < 16; i++)
        {
            stepButtons[i]->setPointer([this, i]() -> void
                                       { (*currentInstrument)->notes[i + currentPage * 16].toggle(); });
            stepButtons[i]->isReleaseSensitive = false;
        }
        break;
    }
    case ACC_MODE:
    case LENGTH_MODE:
    {

        for (byte i = 0; i < 16; i++)
        {
            stepButtons[i]->setPointer(&altStepButtonState[i]);
            stepButtons[i]->isReleaseSensitive = true;
        }
        break;
    }
    default:
        break;
    }
}

void Interface::setup()
{
    setupPins();

    for (byte i = 0; i < 16; i++)
    {
        stepButtons[i] = new Button([this, i]() -> void
                                    { (*currentInstrument)->notes[i + 16 * currentPage].toggle(); });
        if (i >= 8)
            continue;

        controllButtons[i] = new Button(tempControlButtonState + i, true);
        instButtons[i] = new Button(instButtonState + i, true);
        instPots[i] = new Potentiometer([this, i](byte value) -> void
                                        { instruments[i].velocity = value; });
    }
}

byte Interface::getInstColor(bool value)
{
    return value ? C_YELLOW : C_BLUE;
}

byte Interface::getStepColor(bool value, bool stepping)
{
    if (stepping && value)
        return C_WHITE;
    if (stepping)
        return C_CYAN;
    if (value)
        return C_RED;
    return C_BLACK;
    // return (value * C_RED) | (stepping * C_CYAN);
}

void Interface::detectInstrumentChange(byte i)
{
    if (instButtonState[i] && (i != *currentInstrumentIndex))
    {
        sendMessage(MSG_CHANGE_INSTRUMENT, i);
    }
}

void Interface::unpressControlButtons()
{
    for (byte i = 4; i < 8; i++)
    {
        controllButtons[i]->unpress();
    }
}

void Interface::detectCommand()
{
    bool clear = tempControlButtonState[4];
    bool shift = tempControlButtonState[5];
    bool acc = tempControlButtonState[6];
    bool duplicate = tempControlButtonState[7];

    if (clear)
    {
        if (shift)
        {
            sendMessage(MSG_CLEAR_ALL, 0);
            unpressControlButtons();
        }
        else
        {
            for (byte i = 0; i < 8; ++i)
            {
                if (instButtonState[i])
                {
                    sendMessage(MSG_CLEAR_INSTRUMENT, i);
                    unpressControlButtons();
                    break;
                }
            }
        }
    }

    if (shift)
    {
        setStepsPointersMode(LENGTH_MODE);
        for (byte i = 0; i < 16; ++i)
        {
            if (altStepButtonState[i])
                sendMessage(MSG_CHANGE_LENGTH, i + 1 + currentPage * 16);

            if (i < 8 && instButtonState[i])
            {
                sendMessage(MSG_TOGGLE_MUTE_INSTRUMENT, i);
                instButtons[i]->unpress();
                // unpressControlButtons();
                // break;
            }
        }
    }

    if (acc)
    {
        setStepsPointersMode(ACC_MODE);
        for (byte i = 0; i < 16; i++)
        {
            if (altStepButtonState[i])
            {
                (*currentInstrument)->notes[i + 16 * currentPage].toggleAcc();
                stepButtons[i]->unpress();
            }
        }
    }

    if (duplicate)
    {
        unpressControlButtons();
        sendMessage(MSG_DUPLICATE_LOOP, 0);
    }

    if (!shift && !acc)
    {
        setStepsPointersMode(NORMAL_MODE);
    }
}

void Interface::writeLedMode(byte i)
{
    switch (stepMode)
    {
    case ACC_MODE:
    case NORMAL_MODE:
    {
        bool velocity = (*currentInstrument)->notes[i + 16 * currentPage].velocity;
        if ((!velocity && (updateCounter % 4 == 0)) || velocity)
        {
            writeStepLed(getStepColor(
                (*currentInstrument)->notes[i + 16 * currentPage].gate,
                ((*currentInstrument)->currentNote) == (i + 16 * currentPage)));
        }
        break;
    }
    case LENGTH_MODE:
        writeStepLed((i + 16 * currentPage) < (*currentInstrument)->sequenceLength ? C_WHITE : C_BLACK);
        break;
    default:
        break;
    }
}

void Interface::writeInstrumentLed(byte i)
{
    if (instruments[i].isMuted)
        return writeInstLed(C_BLACK);

    writeInstLed(getInstColor(*currentInstrumentIndex == i));
}

void Interface::detectPageChange()
{
    for (byte i = 0; i < 4; i++)
    {
        if (tempControlButtonState[i])
        {
            currentPage = i;
            break;
        }
    }
}

void Interface::update()
{
    int now = millis();
    for (byte i = 0; i < 16; ++i)
    {
        selectChannel(i);
        delayMicroseconds(100);
        stepButtons[i]->setReading(getSteps());
        writeLedMode(i);
        if (i < 8)
        {
            controllButtons[i]->setReading(getControlButtons());
            instButtons[i]->setReading(getInstButtons());
            instPots[i]->setReading(getPotValue(), now);
            writeInstrumentLed(i);
            detectInstrumentChange(i);
            writePageLed(i == currentPage);
        }
        delayMicroseconds(100);
        writeInstLed(C_BLACK);
        writePageLed(C_BLACK);
        writeStepLed(C_BLACK);
        delayMicroseconds(100);
    }
    detectCommand();
    detectPageChange();
    updateCounter = (updateCounter + 1) % 16;
}