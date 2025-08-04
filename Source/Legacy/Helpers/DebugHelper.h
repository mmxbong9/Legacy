﻿#pragma once

namespace Debug
{
	static void Print(const FString& InMsg, int32 InKey = -1, const FColor& InColor = FColor::MakeRandomColor())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, InColor, InMsg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *InMsg);
		}
	}
}