#pragma once

#include "Bomb/Bomb.h"
#include "Game/RoundGameMode.h"
#include "BombDefuse.generated.h"

UCLASS()
class FPSTEMPLATE_API ABombDefuse : public ARoundGameMode
{
	GENERATED_BODY()

	TSubclassOf<ABomb> BombClass;
	
public:
	ABombDefuse(const FObjectInitializer & ObjectInitializer);

	virtual void OnRoundTimeExpired() override;

	virtual void NextRound() override;

	virtual void OnPlayerDied(ABasePlayerController * PC) override;

	virtual void OnBombArmed();
	
	
};
