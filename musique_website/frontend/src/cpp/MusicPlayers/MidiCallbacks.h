#pragma once

#include <vector>

class MidiCallbacks
{
public:
	static MidiCallbacks& GetInstance();

private:
	MidiCallbacks() {}

	MidiCallbacks(MidiCallbacks const&) = delete;
	void operator=(MidiCallbacks const&) = delete;

public:

    void WriteMidi(const uint8_t* bytes, int size);

private:

	void (*WriteMidiCallback)(const uint8_t* bytes, int size);

public:

	friend void AddAudioCallbacksToCpp(int);
};
