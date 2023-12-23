#include "Callbacks.h"

void UpdateRender(const RenderData& renderData) {}

void UpdateFrameData(const FrameData& frameData) {}

void UpdateSongData(const SongData& songData) {}

void UpdatePrintRenderData(const PrintRenderData& printRenderData) {}

void UpdateViewModelData(const ViewModelData& viewModelData) {}

void WriteMidi(char* bytes, size_t size) {}
void SetMidiVolume(int volume) {}
void SetMidiReverb(int reverb) {}

void OnTempoChangedCallback(float tempo) {}