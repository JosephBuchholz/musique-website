#ifndef MUSIQUE_CALLBACKS_H
#define MUSIQUE_CALLBACKS_H

#include "Rendering/RenderData.h"
#include "Rendering/PrintRenderData.h"
#include "FrameData/FrameData.h"
#include "ViewModelData.h"
#include "MusicData/SongData.h"
#include "Editor/Properties.h"

void UpdateRender(const RenderData& renderData);

void UpdateFrameData(const FrameData& frameData);

void UpdateSongData(const SongData& songData);

void UpdatePrintRenderData(const PrintRenderData& printRenderData);

void UpdateViewModelData(const ViewModelData& viewModelData);

void WriteMidi(char* bytes, size_t size);
void SetMidiVolume(int volume);
void SetMidiReverb(int reverb);

void OnTempoChangedCallback(float tempo);

class Callbacks
{
public:
	static Callbacks& GetInstance();

private:
	Callbacks() {}

	Callbacks(Callbacks const&) = delete;
	void operator=(Callbacks const&) = delete;

public:
    void DownloadText(const std::string& name, const std::string& data);

    void UpdateProperties(const Properties& properties);

private:
	void (*DownloadTextCallback)(const char* name, const char* data);
	void (*UpdatePropertiesCallback)(const char* data);

public:

	friend void AddCallbackFunctionsToCpp(int, int);
};

#endif // MUSIQUE_CALLBACKS_H