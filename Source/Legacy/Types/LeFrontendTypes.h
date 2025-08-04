#pragma once

UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	Ok       = 0 UMETA(DisplayName="Ok"),
	YesNo    = 1 UMETA(DisplayName="YesNo"),
	OkCancel = 2 UMETA(DisplayName="OkCancel"),
	Max          UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
	Confirmed = 0 UMETA(DisplayName="Confirmed"),
	Cancelled = 1 UMETA(DisplayName="Cancelled"),
	Closed    = 2 UMETA(DisplayName="Closed"),
	Max           UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EOptionsListDataModifyReason : uint8
{
	DirectlyModified   = 0 UMETA(DisplayName="DirectlyModified"),
	DependencyModified = 1 UMETA(DisplayName="DependencyModified"),
	ResetToDefault     = 2 UMETA(DisplayName="ResetToDefault"),
	Max                    UMETA(Hidden)
};
