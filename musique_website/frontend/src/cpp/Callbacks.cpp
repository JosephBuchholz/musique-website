#include "Callbacks.h"
#include "Debugging/Debug.h"

void UpdateRender(const RenderData& renderData)
{
    LOGD("Callbacks: Update Render function called");
}

void UpdateFrameData(const FrameData& frameData)
{
    LOGD("Callbacks: Update Frame Data function called");
}

void UpdateSongData(const SongData& songData)
{
    LOGD("Callbacks: Update Song Data function called");
}

void UpdatePrintRenderData(const PrintRenderData& printRenderData)
{
    LOGD("Callbacks: Update Print Render Data function called");
}

void UpdateViewModelData(const ViewModelData& viewModelData)
{
    LOGD("Callbacks: Update View Model Data function called");
}

void WriteMidi(char* bytes, size_t size)
{
    LOGD("Callbacks: Update Write Midi function called");
}

void SetMidiVolume(int volume)
{
    LOGD("Callbacks: Update Set Midi Volume function called");
}

void SetMidiReverb(int reverb)
{
    LOGD("Callbacks: Update Set Midi Reverb function called");
}

void OnTempoChangedCallback(float tempo)
{
    LOGD("Callbacks: Update On Tempo Changed function called");
}