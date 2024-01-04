#include "MidiCallbacks.h"

MidiCallbacks& MidiCallbacks::GetInstance()
{
    static MidiCallbacks instance;
    return instance;
}

void MidiCallbacks::WriteMidi(const uint8_t *bytes, int size)
{
    WriteMidiCallback(bytes, size);
}
