// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "LegacyGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class LEGACY_API ULegacyGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	ULegacyGameUserSettings();
	
	static ULegacyGameUserSettings* Get();

	//***** Gameplay Collection Tab *****//
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewDifficulty) { CurrentGameDifficulty = InNewDifficulty; }
	//***** Gameplay Collection Tab *****//
	
	//***** Audio Collection Tab *****//
	UFUNCTION() float GetOverallVolume() const { return OverallVolume; }
	UFUNCTION() float GetMusicVolume  () const { return MusicVolume; }
	UFUNCTION() float GetSoundFXVolume() const { return SoundFXVolume; }
	UFUNCTION() bool  GetAllowBackgroundAudio() const { return bAllowBackgroundAudio; }
	UFUNCTION() bool  GetUseHDRAudioMode() const { return bUseHDRAudioMode; }
	
	UFUNCTION() void SetOverallVolume(float InVolume);
	UFUNCTION() void SetMusicVolume  (float InVolume);
	UFUNCTION() void SetSoundFXVolume(float InVolume);
	UFUNCTION() void SetAllowBackgroundAudio(bool bIsAllowed);
	UFUNCTION() void SetUseHDRAudioMode(bool bIsAllowed);
	//***** Audio Collection Tab *****//
	
	//***** Video Collection Tab *****//
	UFUNCTION() float GetCurrentDisplayGamma() const;

	UFUNCTION() void SetCurrentDisplayGamma(float InNewGamma);
	//***** Video Collection Tab *****//

private:
	//***** Gameplay Collection Tab *****//
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	//***** Gameplay Collection Tab *****//
	
	//***** Audio Collection Tab *****//
	UPROPERTY(Config) float OverallVolume;
	UPROPERTY(Config) float MusicVolume;
	UPROPERTY(Config) float SoundFXVolume;
	UPROPERTY(Config) bool  bAllowBackgroundAudio;
	UPROPERTY(Config) bool  bUseHDRAudioMode;
	//***** Audio Collection Tab *****//
	
	//***** Video Collection Tab *****//
	//***** Video Collection Tab *****//
};
