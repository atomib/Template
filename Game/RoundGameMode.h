#pragma once

#include "Game/RoundGameState.h"
#include "Game/FPSGameMode.h"
#include "RoundGameMode.generated.h"

// A class for round based game modes
UCLASS()
class FPSTEMPLATE_API ARoundGameMode : public AFPSGameMode
{
	GENERATED_BODY()

	FTimerHandle TimerHande_ChangeState;

public:
	int32 NumRoundsPerHalf;

	int32 RoundLength;

	ARoundGameMode(const FObjectInitializer & ObjectInitializer);

	virtual void InitGameState() override;

	ERoundStateEnum GetRoundState() const;
	
	bool HasRoundStarted() const { return GetRoundState() > ERoundStateEnum::PreRound; }

	virtual bool CanPlayerSpawn(ABasePlayerController * PC) const override;

	virtual void MovePlayerToEntryState(ABasePlayerController * PC) override;

	virtual void OnTimeExpired() override;

	// End warm up and start match
	virtual void EndWarmUp();

	// End current round, will start a new round or end the game
	virtual void EndRound(ETeamEnum RoundWinner);

	// Setup everything for a new round, e.g. reset the level
	virtual void NextRound();

	// Start round, i.e. reenable movement for players
	virtual void StartRound();

	virtual void OnRoundTimeExpired();

	// Enter half time state
	virtual void EnterHalfTime();

	// Leave half tim state
	virtual void ExitHalfTime();

	virtual void OnPlayerDied(ABasePlayerController * PC) override;
	
	
};
