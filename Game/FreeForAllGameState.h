#pragma once

#include "Game/FPSGameState.h"
#include "FreeForAllGameState.generated.h"

UCLASS()
class FPSTEMPLATE_API AFreeForAllGameState : public AFPSGameState
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
		int32 MaxKills;
	
public:
	AFreeForAllGameState();

	void SetMaxKills(int32 Val);

	UFUNCTION(BlueprintPure, Category = GameState)
		int32 GetMaxKills() const { return MaxKills; };
	
};
