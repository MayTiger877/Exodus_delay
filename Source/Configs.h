/*
  ==============================================================================

    Configs.h
    Created: 24 Oct 2025 4:17:11pm
    Author:  MayTigerStation

  ==============================================================================
*/

#pragma once

// set defines for different configurations

inline const int SCREEN_WIDTH = 975;
inline const int SCREEN_HEIGHT = 650;

inline const int FPS = 60;

inline const float DEFAULT_BPM = 120.0f;

// Slider values and positions

inline const int TILE_SLIDER_WIDTH = 28;
inline const int TILE_SLIDER_HEIGHT = 11;
inline const int TILE_SLIDER_BG_WIDTH = 40;
inline const int TILE_SLIDER_BG_HEIGHT = 67;
inline const int TILE_SLIDER_STARTING_X = 145;
inline const int TILE_SLIDER_ENDING_X = 950;
inline const int TILE_SLIDER_GAP_X = 51;

inline const int COSTUME_KNOB_WIDTH = 46;
inline const int COSTUME_KNOB_HEIGHT = 46;

inline const int GAIN_SLIDER_Y = 68;
inline const float GAIN_SLIDER_DEFAULT_VALUE = 1.0f;
inline const float GAIN_SLIDER_MIN_VALUE = 0.0f;
inline const float GAIN_SLIDER_MAX_VALUE = 1.0f;
inline const float GAIN_SLIDER_INTERVAL = 0.1f;

inline const int PAN_SLIDER_Y = 138;
inline const float PAN_SLIDER_DEFAULT_VALUE = 0.0f;
inline const float PAN_SLIDER_MIN_VALUE = -1.0f;
inline const float PAN_SLIDER_MAX_VALUE = 1.0f;
inline const float PAN_SLIDER_INTERVAL = 0.2f;

inline const int DISTORTION_MIX_SLIDER_Y = 208;
inline const float DISTORTION_MIX_SLIDER_DEFAULT_VALUE = 1.0f;
inline const float DISTORTION_MIX_SLIDER_MIN_VALUE = 0.0f;
inline const float DISTORTION_MIX_SLIDER_MAX_VALUE = 1.0f;
inline const float DISTORTION_MIX_SLIDER_INTERVAL = 0.1f;

inline const int PHASER_MIX_SLIDER_Y = 278;
inline const float PHASER_MIX_SLIDER_DEFAULT_VALUE = 0.0f;
inline const float PHASER_MIX_SLIDER_MIN_VALUE = 0.0f;
inline const float PHASER_MIX_SLIDER_MAX_VALUE = 1.0f;
inline const float PHASER_MIX_SLIDER_INTERVAL = 0.1f;

inline const int REVERB_MIX_SLIDER_Y = 348;
inline const float REVERB_MIX_SLIDER_DEFAULT_VALUE = 0.0f;
inline const float REVERB_MIX_SLIDER_MIN_VALUE = 0.0f;
inline const float REVERB_MIX_SLIDER_MAX_VALUE = 1.0f;
inline const float REVERB_MIX_SLIDER_INTERVAL = 0.1f;

inline const int TIME_SYNCED_SLIDER_DEFAULT_VALUE = 3;
inline const int TIME_SYNCED_SLIDER_MIN_VALUE = 0;
inline const int TIME_SYNCED_SLIDER_MAX_VALUE = 7;
inline const int TIME_SYNCED_SLIDER_INTERVAL = 1;


// General Settings

inline const juce::Rectangle<int> GNRL_DELAY_TIME_SLIDER_BOUNDS = {753, 500, COSTUME_KNOB_WIDTH, COSTUME_KNOB_HEIGHT};
inline const float GNRL_DELAY_TIME_SLIDER_DEFAULT_VALUE = 0.6f;
inline const float GNRL_DELAY_TIME_SLIDER_MIN_VALUE = 0.2f;
inline const float GNRL_DELAY_TIME_SLIDER_MAX_VALUE = 2.0f;
inline const float GNRL_DELAY_TIME_SLIDER_INTERVAL = 0.01f;

inline const int GNRL_BY_TEMPO_TOGGLE_WIDTH = 13;
inline const int GNRL_BY_TEMPO_TOGGLE_HEIGHT = 40;
inline const juce::Rectangle<int> GNRL_BY_TEMPO_TOGGLE_BOUNDS = {830, 500, GNRL_BY_TEMPO_TOGGLE_WIDTH, GNRL_BY_TEMPO_TOGGLE_HEIGHT};
inline const bool GNRL_BY_TEMPO_TOGGLE_DEFAULT_VALUE = false;

inline const juce::Rectangle<int> GNRL_FEEDBACK_SLIDER_BOUNDS = {895, 500, COSTUME_KNOB_WIDTH, COSTUME_KNOB_HEIGHT};
inline const float GNRL_FEEDBACK_SLIDER_DEFAULT_VALUE = 0.7f;
inline const float GNRL_FEEDBACK_SLIDER_MIN_VALUE = 0.0f;
inline const float GNRL_FEEDBACK_SLIDER_MAX_VALUE = 0.95f;
inline const float GNRL_FEEDBACK_SLIDER_INTERVAL = 0.01f;

inline const juce::Rectangle<int> GNRL_DRY_LEVEL_SLIDER_BOUNDS = {752, 580, COSTUME_KNOB_WIDTH, COSTUME_KNOB_HEIGHT};
inline const float GNRL_DRY_LEVEL_SLIDER_DEFAULT_VALUE = 0.8f;
inline const float GNRL_DRY_LEVEL_SLIDER_MIN_VALUE = 0.0f;
inline const float GNRL_DRY_LEVEL_SLIDER_MAX_VALUE = 1.0f;
inline const float GNRL_DRY_LEVEL_SLIDER_INTERVAL = 0.01f;

inline const juce::Rectangle<int> GNRL_WET_LEVEL_SLIDER_BOUNDS = {895, 580, COSTUME_KNOB_WIDTH, COSTUME_KNOB_HEIGHT};
inline const float GNRL_WET_LEVEL_SLIDER_DEFAULT_VALUE = 0.8f;
inline const float GNRL_WET_LEVEL_SLIDER_MIN_VALUE = 0.0f;
inline const float GNRL_WET_LEVEL_SLIDER_MAX_VALUE = 1.0f;
inline const float GNRL_WET_LEVEL_SLIDER_INTERVAL = 0.01f;

// Distortion Settings

typedef enum
{
    distType_SoftClip = 0,
    distType_HardClip = 1,
    distType_Exponential = 2,
    distType_Foldback = 3,
    distType_numberOfTypes = 4
}DistortionType;

inline const int DISTORTION_KNOB_WIDTH = 34;
inline const int DISTORTION_KNOB_HEIGHT = 24;

inline const juce::Rectangle<int> DISTORTION_TYPE_SLIDER_BOUNDS {30, 490, 40, 117};
inline const float DISTORTION_TYPE_SLIDER_DEFAULT_VALUE = 0.0f;
inline const float DISTORTION_TYPE_SLIDER_MIN_VALUE = 0.0f;
inline const float DISTORTION_TYPE_SLIDER_MAX_VALUE = 3.0f;
inline const float DISTORTION_TYPE_SLIDER_INTERVAL = 1.0f;

inline const juce::Rectangle<int> DISTORTION_DRIVE_SLIDER_BOUNDS {164, 500, COSTUME_KNOB_WIDTH, COSTUME_KNOB_HEIGHT};
inline const float DISTORTION_DRIVE_SLIDER_DEFAULT_VALUE = 2.0f;
inline const float DISTORTION_DRIVE_SLIDER_MIN_VALUE = 1.0f;
inline const float DISTORTION_DRIVE_SLIDER_MAX_VALUE = 7.0f;
inline const float DISTORTION_DRIVE_SLIDER_INTERVAL = 0.1f;

inline const juce::Rectangle<int> DISTORTION_THRESHOLD_SLIDER_BOUNDS {164, 580, COSTUME_KNOB_WIDTH, COSTUME_KNOB_HEIGHT};
inline const float DISTORTION_THRESHOLD_SLIDER_DEFAULT_VALUE = 0.5f;
inline const float DISTORTION_THRESHOLD_SLIDER_MIN_VALUE = 0.0f;
inline const float DISTORTION_THRESHOLD_SLIDER_MAX_VALUE = 1.0f;
inline const float DISTORTION_THRESHOLD_SLIDER_INTERVAL = 0.01f;


// Phaser Settings

typedef enum
{
    phaserLFOType_Sine = 0,
    phaserLFOType_Triangle = 1,
    phaserLFOType_SawUp = 2,
    phaserLFOType_SawDown = 3,
    phaserLFOType_Square = 4,
    phaserLFOType_numberOfTypes = 5
}PhaserLFOType;

inline const juce::Rectangle<int> PHASER_TYPE_SLIDER_BOUNDS = {267, 579, 68, 50};
inline const float PHASER_TYPE_SLIDER_DEFAULT_VALUE = 0.0f;
inline const float PHASER_TYPE_SLIDER_MIN_VALUE = 0.0f;
inline const float PHASER_TYPE_SLIDER_MAX_VALUE = 4.0f;
inline const float PHASER_TYPE_SLIDER_INTERVAL = 1.0f;

inline const juce::Rectangle<int> PHASER_RATE_SLIDER_BOUNDS = {274, 500, COSTUME_KNOB_WIDTH, COSTUME_KNOB_HEIGHT};
inline const float PHASER_RATE_SLIDER_DEFAULT_VALUE = 4.0f;
inline const float PHASER_RATE_SLIDER_SKEW_MID_POINT = 8.0f;
inline const float PHASER_RATE_SLIDER_MIN_VALUE = 1.0f;
inline const float PHASER_RATE_SLIDER_MAX_VALUE = 50.0f;

inline const int PHASER_BY_TEMPO_TOGGLE_WIDTH = 13;
inline const int PHASER_BY_TEMPO_TOGGLE_HEIGHT = 40;
inline const juce::Rectangle<int> PHASER_BY_TEMPO_TOGGLE_BOUNDS = {351, 500, PHASER_BY_TEMPO_TOGGLE_WIDTH, PHASER_BY_TEMPO_TOGGLE_HEIGHT};
inline const bool PHASER_BY_TEMPO_TOGGLE_DEFAULT_VALUE = false;

inline const juce::Rectangle<int> PHASER_DEPTH_SLIDER_BOUNDS = {415, 500, COSTUME_KNOB_WIDTH, COSTUME_KNOB_HEIGHT};
inline const float PHASER_DEPTH_SLIDER_DEFAULT_VALUE = 0.5f;
inline const float PHASER_DEPTH_SLIDER_MIN_VALUE = 0.0f;
inline const float PHASER_DEPTH_SLIDER_MAX_VALUE = 1.0f;
inline const float PHASER_DEPTH_SLIDER_INTERVAL = 0.01f;

inline const juce::Rectangle<int> PHASER_FREQ_SLIDER_BOUNDS = {415, 580, COSTUME_KNOB_WIDTH, COSTUME_KNOB_HEIGHT};
inline const float PHASER_FREQ_SLIDER_SKEW_MID_POINT = 3000.0f;
inline const float PHASER_FREQ_SLIDER_DEFAULT_VALUE = 4000.0f;
inline const float PHASER_FREQ_SLIDER_MIN_VALUE = 600.0f;
inline const float PHASER_FREQ_SLIDER_MAX_VALUE = 10000.0f;

// Reverb Settings;

inline const juce::Rectangle<int> REVERB_ROOM_SIZE_SLIDER_BOUNDS = {513, 500, COSTUME_KNOB_WIDTH, COSTUME_KNOB_HEIGHT};
inline const float REVERB_ROOM_SIZE_SLIDER_DEFAULT_VALUE = 0.5f;
inline const float REVERB_ROOM_SIZE_SLIDER_MIN_VALUE = 0.0f;
inline const float REVERB_ROOM_SIZE_SLIDER_MAX_VALUE = 1.0f;
inline const float REVERB_ROOM_SIZE_SLIDER_INTERVAL = 0.01f;

inline const juce::Rectangle<int> REVERB_DAMPING_SLIDER_BOUNDS = {655, 500, COSTUME_KNOB_WIDTH, COSTUME_KNOB_HEIGHT};
inline const float REVERB_DAMPING_SLIDER_DEFAULT_VALUE = 0.5f;
inline const float REVERB_DAMPING_SLIDER_MIN_VALUE = 0.0f;
inline const float REVERB_DAMPING_SLIDER_MAX_VALUE = 1.0f;
inline const float REVERB_DAMPING_SLIDER_INTERVAL = 0.01f;

inline const juce::Rectangle<int> REVERB_WIDTH_SLIDER_BOUNDS = {587, 580, COSTUME_KNOB_WIDTH, COSTUME_KNOB_HEIGHT};
inline const float REVERB_WIDTH_SLIDER_DEFAULT_VALUE = 0.5f;
inline const float REVERB_WIDTH_SLIDER_MIN_VALUE = 0.0f;
inline const float REVERB_WIDTH_SLIDER_MAX_VALUE = 1.0f;
inline const float REVERB_WIDTH_SLIDER_INTERVAL = 0.01f;