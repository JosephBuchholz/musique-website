#ifndef MUSIQUE_CALLBACKS_H
#define MUSIQUE_CALLBACKS_H

#include "Rendering/RenderData.h"
#include "Rendering/PrintRenderData.h"
#include "FrameData/FrameData.h"
#include "ViewModelData.h"
#include "MusicData/SongData.h"

void UpdateRender(const RenderData& renderData);

void UpdateFrameData(const FrameData& frameData);

void UpdateSongData(const SongData& songData);

void UpdatePrintRenderData(const PrintRenderData& printRenderData);

void UpdateViewModelData(const ViewModelData& viewModelData);

void WriteMidi(char* bytes, size_t size);
void SetMidiVolume(int volume);
void SetMidiReverb(int reverb);

void OnTempoChangedCallback(float tempo);

#endif // MUSIQUE_CALLBACKS_H