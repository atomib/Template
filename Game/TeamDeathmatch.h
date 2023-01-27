#pragma once

#include "Game/FPSGameMode.h"
#include "TeamDeathmatch.generated.h"

UCLASS()
class FPSTEMPLATE_API ATeamDeathmatch : public AFPSGameMode
{
	GENERATED_BODY()
	
public:
	ATeamDeathmatch(const FObjectInitializer & ObjectInitializer);

protected:
	virtual void InitGameState() override;

	virtual void OnPlayerDied(ABasePlayerController * PC) override;
	
	
};
