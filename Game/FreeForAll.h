#pragma once

#include "Game/FPSGameMode.h"
#include "FreeForAll.generated.h"

UCLASS()
class FPSTEMPLATE_API AFreeForAll : public AFPSGameMode
{
	GENERATED_BODY()

public:
	AFreeForAll(const FObjectInitializer & ObjectInitializer);
	
	virtual void InitGameState() override;

	virtual void OnPlayerKilledEnemy(const FDamageEvent & DamageEvent, ABasePlayerController * PC, ABasePlayerController * OtherPC) override;
	
};
