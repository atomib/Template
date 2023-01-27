#pragma once

#include "Player/BasePlayerState.h"
#include "GameFramework/GameState.h"
#include "FPSGameState.generated.h"

class ABasePlayerState;

UCLASS()
class FPSTEMPLATE_API AFPSGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = GameState)
		ETeamEnum MatchWinner;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = GameState)
		ABasePlayerState * WinnerPlayerState;

	// Remaining time seconds for the match/round
	UPROPERTY(Replicated, BlueprintReadOnly, Category = GameState)
		int32 TimeRemaining;
	
	void DefaultTimer() override;
	
};
