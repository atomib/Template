#pragma once

#include "Game/FPSGameMode.h"
#include "Conquest.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AConquest : public AFPSGameMode
{
	GENERATED_BODY()
	
public:
	AConquest(const FObjectInitializer & ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void InitGameState() override;

	virtual void OnPlayerDied(ABasePlayerController * PC) override;

private:
	virtual void TickFlags();
	
	
};
