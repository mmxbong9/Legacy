// bong9 All Rights Reserved

#include "LeGameInstance.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Helpers/DebugHelper.h"

void ULeGameInstance::CancelPending()
{
	UWorld* World = GetWorld();
	
	check(World && GEngine && WorldContext)

	if (IsValid(WorldContext->PendingNetGame.Get()))
	{
		// 현재 소켓연결 시도 중단.
		GEngine->CancelPending(World, WorldContext->PendingNetGame.Get());

		// Tick 에서 접근 하지 못하도록 초기화.
		WorldContext->PendingNetGame = nullptr;

		// 해당 드라이버 객체 즉시 제거.
		GEngine->DestroyNamedNetDriver(GetWorld(), NAME_PendingNetDriver);
	}
}
