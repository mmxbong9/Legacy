// bong9 All Rights Reserved

#include "LeListDataObject_StringResolution.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Legacy/Settings/LegacyGameUserSettings.h"
#include "Legacy/Helpers/DebugHelper.h"
#include "Legacy/UI/Widgets/Options/OptionsDataInteractionHelper.h"

void ULeListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions;
	
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);

	AvailableResolutions.Sort(
	[](const FIntPoint& A, const FIntPoint& B)->bool
		{
			return A.SizeSquared() < B.SizeSquared();
		}
	);
	
	for (const FIntPoint& Resolution : AvailableResolutions)
	{
		AddDynamicOption(ResToValueString(Resolution), ResToDisplayText(Resolution));
	}

	MaximumAllowedResolution = ResToValueString(AvailableResolutions.Last());

	SetDefaultValueFromString(MaximumAllowedResolution);
}

void ULeListDataObject_StringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = ResToDisplayText(ULegacyGameUserSettings::Get()->GetScreenResolution());
	}
}

FString ULeListDataObject_StringResolution::ResToValueString(const FIntPoint& InResolution) const
{
	// Resolution Value from Dynamic Getter: (X=1920,Y=1080)
	return FString::Printf(TEXT("X=%i, Y=%i"), InResolution.X, InResolution.Y);
}

FText ULeListDataObject_StringResolution::ResToDisplayText(const FIntPoint& InResolution) const
{
	const FString DisplayString = FString::Printf(TEXT("%i x %i"), InResolution.X, InResolution.Y);
	
	return FText::FromString(DisplayString);
}
