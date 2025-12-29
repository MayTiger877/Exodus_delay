/*
  ==============================================================================

    Parameters.h
    Created: 18 Dec 2025 11:56:04am
    Author:  MayTigerStation

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

inline const juce::String delayTimeParamID = "GNRL_DELAY_TIME";
inline const juce::String delayTimeParamName = "GNRL_DELAY_TIME_NAME";
inline const juce::String delayTempoSyncParamID = "GNRL_DELAY_TIME_SYNCED";
inline const juce::String delayTempoSyncParamName = "GNRL_DELAY_TIME_SYNCED_NAME";
inline const juce::String delayFeedbackParamID = "GNRL_FEEDBACK";
inline const juce::String delayFeedbackParamName = "GNRL_FEEDBACK_NAME";
inline const juce::String delayDryLevelParamID = "GNRL_DRY_LEVEL";
inline const juce::String delayDryLevelParamName = "GNRL_DRY_LEVEL_NAME";
inline const juce::String delayWetLevelParamID = "GNRL_WET_LEVEL";
inline const juce::String delayWetLevelParamName = "GNRL_WET_LEVEL_NAME";

inline const juce::String distortionTypeParamID = "DISTORTION_TYPE";
inline const juce::String distortionTypeParamName = "DISTORTION_TYPE_NAME";
inline const juce::String distortionDriveParamID = "DISTORTION_DRIVE";
inline const juce::String distortionDriveParamName = "DISTORTION_DRIVE_NAME";
inline const juce::String distortionThresholdParamID = "DISTORTION_THRESHOLD";
inline const juce::String distortionThresholdParamName = "DISTORTION_THRESHOLD_NAME";

inline const juce::String phaserTypeParamID = "PHASER_TYPE";
inline const juce::String phaserTypeParamName = "PHASER_TYPE_NAME";
inline const juce::String phaserRateParamID = "PHASER_RATE";
inline const juce::String phaserRateParamName = "PHASER_RATE_NAME";
inline const juce::String phaserTempoSyncParamID = "PHASER_RATE_SYNCED";
inline const juce::String phaserTempoSyncParamName = "PHASER_RATE_SYNCED_NAME";
inline const juce::String phaserDepthParamID = "PHASER_DEPTH";
inline const juce::String phaserDepthParamName = "PHASER_DEPTH_NAME";
inline const juce::String phaserFreqParamID = "PHASER_FREQ";
inline const juce::String phaserFreqParamName = "PHASER_FREQ_NAME";

inline const juce::String reverbRoomSizeParamID = "REVERB_ROOM_SIZE";
inline const juce::String reverbRoomSizeParamName = "REVERB_ROOM_SIZE_NAME";
inline const juce::String reverbDampingParamID = "REVERB_DAMPING";
inline const juce::String reverbDampingParamName = "REVERB_DAMPING_NAME";
inline const juce::String reverbWidthParamID = "REVERB_WIDTH";
inline const juce::String reverbWidthParamName = "REVERB_WIDTH_NAME";

inline const juce::String generalTempoSyncToggleParamID = "GNRL_TEMPO_SYNC_TOGGLE";
inline const juce::String generalTempoSyncToggleParamName = "GNRL_DELAY_TOGGLE_NAME";

inline const juce::String phaserTempoSyncToggleParamID = "PHASER_TEMPO_SYNC_TOGGLE";
inline const juce::String phaserTempoSyncToggleParamName = "PHASER_TEMPO_SYNC_TOGGLE_NAME";

inline const juce::String gainMixParamID[16] = {
    "GAIN_0", "GAIN_1", "GAIN_2", "GAIN_3",
    "GAIN_4", "GAIN_5", "GAIN_6", "GAIN_7",
    "GAIN_8", "GAIN_9", "GAIN_10", "GAIN_11",
    "GAIN_12", "GAIN_13", "GAIN_14", "GAIN_15"
};

inline const juce::String panMixParamID[16] = {
    "PAN_0", "PAN_1", "PAN_2", "PAN_3",
    "PAN_4", "PAN_5", "PAN_6", "PAN_7",
    "PAN_8", "PAN_9", "PAN_10", "PAN_11",
    "PAN_12", "PAN_13", "PAN_14", "PAN_15"
};

inline const juce::String distortionMixParamID[16] = {
    "DISTORTION_MIX_0", "DISTORTION_MIX_1", "DISTORTION_MIX_2", "DISTORTION_MIX_3",
    "DISTORTION_MIX_4", "DISTORTION_MIX_5", "DISTORTION_MIX_6", "DISTORTION_MIX_7",
    "DISTORTION_MIX_8", "DISTORTION_MIX_9", "DISTORTION_MIX_10", "DISTORTION_MIX_11",
    "DISTORTION_MIX_12", "DISTORTION_MIX_13", "DISTORTION_MIX_14", "DISTORTION_MIX_15"
};

inline const juce::String reverbMixParamID[16] = {
    "REVERB_MIX_0", "REVERB_MIX_1", "REVERB_MIX_2", "REVERB_MIX_3",
    "REVERB_MIX_4", "REVERB_MIX_5", "REVERB_MIX_6", "REVERB_MIX_7",
    "REVERB_MIX_8", "REVERB_MIX_9", "REVERB_MIX_10", "REVERB_MIX_11",
    "REVERB_MIX_12", "REVERB_MIX_13", "REVERB_MIX_14", "REVERB_MIX_15"
};

inline const juce::String phaserMixParamID[16] = {
    "PHASER_MIX_0", "PHASER_MIX_1", "PHASER_MIX_2", "PHASER_MIX_3",
    "PHASER_MIX_4", "PHASER_MIX_5", "PHASER_MIX_6", "PHASER_MIX_7",
    "PHASER_MIX_8", "PHASER_MIX_9", "PHASER_MIX_10", "PHASER_MIX_11",
    "PHASER_MIX_12", "PHASER_MIX_13", "PHASER_MIX_14", "PHASER_MIX_15"
};

struct parameters
{
    explicit parameters(juce::AudioProcessorValueTreeState& apvts)
    {
		delayTimeParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(delayTimeParamID));
		delayTempoSyncParam = dynamic_cast<juce::AudioParameterInt*>(apvts.getParameter(delayTempoSyncParamID));
		delayFeedbackParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(delayFeedbackParamID));
		delayDryLevelParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(delayDryLevelParamID));
		delayWetLevelParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(delayWetLevelParamID));

		distortionTypeParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(distortionTypeParamID));
		distortionDriveParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(distortionDriveParamID));
		distortionThresholdParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(distortionThresholdParamID));

		phaserTypeParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(phaserTypeParamID));
		phaserRateParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(phaserRateParamID));
        phaserTempoSyncParam = dynamic_cast<juce::AudioParameterInt*>(apvts.getParameter(phaserTempoSyncParamID));
		phaserDepthParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(phaserDepthParamID));
		phaserFreqParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(phaserFreqParamID));

		reverbRoomSizeParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(reverbRoomSizeParamID));
		reverbDampingParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(reverbDampingParamID));
        reverbWidthParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(reverbWidthParamID));
        
        for (int i = 0; i < 16; ++i)
        {
            gainMixParam[i] = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(gainMixParamID[i]));
            panMixParam[i] = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(panMixParamID[i]));
            distortionMixParam[i] = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(distortionMixParamID[i]));
            reverbMixParam[i] = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(reverbMixParamID[i]));
            phaserMixParam[i] = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(phaserMixParamID[i]));
		}

		generalTempoSyncToggleParam = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(generalTempoSyncToggleParamID));
		phaserTempoSyncToggleParam = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(phaserTempoSyncToggleParamID));
	}

    juce::AudioParameterFloat* delayTimeParam { nullptr };
    juce::AudioParameterInt* delayTempoSyncParam{ nullptr };
    juce::AudioParameterFloat* delayFeedbackParam{ nullptr };
	juce::AudioParameterFloat* delayDryLevelParam{ nullptr };
	juce::AudioParameterFloat* delayWetLevelParam{ nullptr };

	juce::AudioParameterFloat* distortionTypeParam{ nullptr };
	juce::AudioParameterFloat* distortionDriveParam{ nullptr };
    juce::AudioParameterFloat* distortionThresholdParam{ nullptr };

	juce::AudioParameterFloat* phaserTypeParam{ nullptr };
	juce::AudioParameterFloat* phaserRateParam{ nullptr };
    juce::AudioParameterInt* phaserTempoSyncParam{ nullptr };
	juce::AudioParameterFloat* phaserDepthParam{ nullptr };
	juce::AudioParameterFloat* phaserFreqParam{ nullptr };

	juce::AudioParameterFloat* reverbRoomSizeParam{ nullptr };
	juce::AudioParameterFloat* reverbDampingParam{ nullptr };
    juce::AudioParameterFloat* reverbWidthParam{ nullptr };

	juce::AudioParameterFloat* gainMixParam[16] { nullptr, nullptr, nullptr, nullptr,
                                                    nullptr, nullptr, nullptr, nullptr,
                                                    nullptr, nullptr, nullptr, nullptr,
													nullptr, nullptr, nullptr, nullptr };

    juce::AudioParameterFloat* panMixParam[16] { nullptr, nullptr, nullptr, nullptr,
                                                   nullptr, nullptr, nullptr, nullptr,
                                                   nullptr, nullptr, nullptr, nullptr,
												   nullptr, nullptr, nullptr, nullptr };

    juce::AudioParameterFloat* distortionMixParam[16]{ nullptr, nullptr, nullptr, nullptr,
                                                        nullptr, nullptr, nullptr, nullptr,
                                                        nullptr, nullptr, nullptr, nullptr,
                                                        nullptr, nullptr, nullptr, nullptr };

	juce::AudioParameterFloat* reverbMixParam[16]{ nullptr, nullptr, nullptr, nullptr,
                                                    nullptr, nullptr, nullptr, nullptr,
                                                    nullptr, nullptr, nullptr, nullptr,
												    nullptr, nullptr, nullptr, nullptr };

    juce::AudioParameterFloat* phaserMixParam[16]{ nullptr, nullptr, nullptr, nullptr,
                                                    nullptr, nullptr, nullptr, nullptr,
													nullptr, nullptr, nullptr, nullptr,
								                    nullptr, nullptr, nullptr, nullptr };

	juce::AudioParameterBool* generalTempoSyncToggleParam{ nullptr };
	juce::AudioParameterBool* phaserTempoSyncToggleParam{ nullptr };

	
};
