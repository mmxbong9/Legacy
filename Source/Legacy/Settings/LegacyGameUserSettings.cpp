// bong9 All Rights Reserved

#include "LegacyGameUserSettings.h"

ULegacyGameUserSettings::ULegacyGameUserSettings()
	: OverallVolume(1.f)
	, MusicVolume(1.f)
	, SoundFXVolume(1.f)
	, bAllowBackgroundAudio(false)
	, bUseHDRAudioMode(false)
{
}

ULegacyGameUserSettings* ULegacyGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<ULegacyGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

void ULegacyGameUserSettings::SetOverallVolume(float InVolume)
{
	OverallVolume = InVolume;

	// The actual logic for controlling the volume goes here 
}

void ULegacyGameUserSettings::SetMusicVolume(float InVolume)
{
	MusicVolume = InVolume;
}

void ULegacyGameUserSettings::SetSoundFXVolume(float InVolume)
{
	SoundFXVolume = InVolume;
}

void ULegacyGameUserSettings::SetAllowBackgroundAudio(bool bIsAllowed)
{
	bAllowBackgroundAudio = bIsAllowed;
}

void ULegacyGameUserSettings::SetUseHDRAudioMode(bool bIsAllowed)
{
	bUseHDRAudioMode = bIsAllowed;
}

float ULegacyGameUserSettings::GetCurrentDisplayGamma() const
{
	if (GEngine)
	{
		return GEngine->GetDisplayGamma();
	}
	
	return 0.0f;
}

void ULegacyGameUserSettings::SetCurrentDisplayGamma(float InNewGamma)
{
	if (GEngine)
	{
		GEngine->DisplayGamma = InNewGamma;
	}
}
