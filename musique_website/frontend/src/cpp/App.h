/**
 * This file defines the class `App` which acts like an interface between Kotlin and C++.
 */

#pragma once

#include <memory>

#include "MusicData/Song.h"
#include "ViewModelData.h"
#include "Settings.h"
#include "Collisions/Vec2.h"
#include "Events/InputEvent.h"
#include "TranspositionRequest.h"

#include "Rendering/MusicRenderer.h"
#include "MusicPlayers/MusicPlayer.h"

/**
 * This class acts like an interface between Kotlin and C++.
 */
class App
{

public:
	static App& GetInstance();

    ~App();

private:
	App();

	App(App const&) = delete;
	void operator=(App const&) = delete;

public:

    void StartRendering() { startRendering = true; }
    void StopRendering() { startRendering = false; }

    /**
     * Handles all the updates for rendering and audio.
     * 
     * @param dt - The delta time in milliseconds in between each call.
    */
    void OnUpdate(double dt);

    void OnPlayButtonToggled(bool state) { if (state) { musicPlayer->OnPlay(); } else { musicPlayer->OnStop(); } }
    void OnResetButtonPressed() { musicPlayer->Reset(); }
    void OnPlayProgressChanged(float progress);
    void UpdateInstrumentInfo(const InstrumentInfo& info, unsigned int index);
    void SetViewModelData(ViewModelData viewModelData);
    void OnMidiStart();
    void LoadSongFromString(const std::string& extension, const std::string& string);
    bool OnUpdatePrintLayout();
    void UpdateSettings(const Settings& s) { settings = s; OnLayoutChanged(); }
    void OnLayoutChanged();
    void OnVolumeChanged(float volume) { if (musicPlayer) musicPlayer->SetVolume(volume); }
    void OnTempoPercentageChanged(float tempoPercentage) { if (musicPlayer) musicPlayer->SetTempoPercentage(tempoPercentage); }
    void OnTranspose(const TranspositionRequest& transpositionRequest);

    void OnMetronomeToggled(bool state) { if (musicPlayer) musicPlayer->OnMetronomeToggled(state); }

    bool OnMouseScrollEvent(const MouseScrollEvent& event);
    bool OnButtonEvent(const ButtonEvent& event);

    int OnCalculateNumPages();

private:

    std::shared_ptr<MusicRenderer> musicRenderer;
    std::shared_ptr<MusicPlayer> musicPlayer;

    bool isUpdating = false;
    bool updateMusicPlayer = true;
    bool allowRendering = true;
    bool allowFrameDataRendering = true;
    bool doCollisions = true;

    int jcount = 0;
    int icount = 0;
    float aTime = 0.0f;
    float fps = 0.0f;
    int frames = 0;

    float playLinePosition = 0.0f;
    float playLineHeight = 0.0f;
    float playLineY = 0.0f;


    std::shared_ptr<Song> song;
    bool songUpdated = false;

    bool songLoaded = false;


    std::string songString = "";

    float displayWidth = 1000.0f;

    float curveStartX = 0.0f;
    float curveStartY = 0.0f;

    float beamStartX = 0.0f;
    float beamStartY = 0.0f;

    bool startRendering = false;

    Settings settings;

    float pageWidth = 1024.0f;

    Vec2<float> pointerPosition = { 0.0f, 0.0f };
};
