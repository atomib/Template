#include "FPSTemplate.h"
#include "RoundGameMode.h"
#include "Game/RoundGameState.h"

ARoundGameMode::ARoundGameMode(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ARoundGameState::StaticClass();

	bUseCustomLoadout = false;
	DefaultLoadout.ItemSlots[1].ItemClassName = FName(TEXT("BP_Sock17_C"));

	NumRoundsPerHalf = 8;
	RoundLength = 60;
}

void ARoundGameMode::InitGameState()
{
	Super::InitGameState();

	ARoundGameState * RoundGameState = Cast<ARoundGameState>(GameState);
	if (RoundGameState)
	{
		RoundGameState->NumRoundsPerHalf = NumRoundsPerHalf;
		// Warmup time
		RoundGameState->TimeRemaining = 10;
	}
}

ERoundStateEnum ARoundGameMode::GetRoundState() const
{
	ARoundGameState * RoundGameState = Cast<ARoundGameState>(GameState);
	if (RoundGameState)
	{
		return RoundGameState->RoundState;
	}

	return ERoundStateEnum::PreRound;
}

bool ARoundGameMode::CanPlayerSpawn(ABasePlayerController * PC) const
{
	if (Super::CanPlayerSpawn(PC))
	{
		if (GetRoundState() == ERoundStateEnum::WarmUp || GetRoundState() == ERoundStateEnum::PreRound)
		{
			return true;
		}

		//ABaseGameState * BaseGameState = Cast<ABaseGameState>(GameState);
		//ABasePlayerState * PS = Cast<ABasePlayerState>(PC->PlayerState);
		//if (BaseGameState && PS)
		//{
		//	// Player may only spawn if round did not start yet or he just joined
		//	return BaseGameState->TimeRemaining == 0 || (BaseGameState->TimeRemaining < 20 && PS->GetDeaths() == 0);
		//}
	}

	return false;
}

void ARoundGameMode::MovePlayerToEntryState(ABasePlayerController * PC)
{
	if (CanPlayerSpawn(PC))
	{
		PlayerSpawn(PC);
	}
	else
	{
		PC->EnterSpectatingState();
	}
}

void ARoundGameMode::OnTimeExpired()
{
	switch (GetRoundState())
	{
	case (ERoundStateEnum::WarmUp) :
		EndWarmUp();
		break;
	case (ERoundStateEnum::PreRound) :
		StartRound();
		break;
	case (ERoundStateEnum::InProgress) :
		OnRoundTimeExpired();
		break;
	case (ERoundStateEnum::RoundEnd) :
		NextRound();
		break;
	}
}

void ARoundGameMode::EndWarmUp()
{
	for (FConstPlayerControllerIterator PlayerItr = GetWorld()->GetPlayerControllerIterator(); PlayerItr; ++PlayerItr)
	{
		ABasePlayerController * PC = Cast<ABasePlayerController>(*PlayerItr);
		if (PC)
		{
			if (PC->GetTeam() == ETeamEnum::None)
			{
				// Force players into teams if they didn't join one yet
				PlayerAutoJoinTeam(PC);
			}

			PC->UnPossess();
			// Reset score for actual match
			PC->ResetScore();
		}
	}

	NextRound();
}

void ARoundGameMode::EndRound(ETeamEnum RoundWinner)
{
	ARoundGameState * RoundGameState = Cast<ARoundGameState>(GameState);
	if (RoundGameState && RoundGameState->RoundState == ERoundStateEnum::InProgress)
	{
		RoundGameState->MatchWinner = RoundWinner;
		RoundGameState->RoundState = ERoundStateEnum::RoundEnd;

		for (FConstPlayerControllerIterator PlayerItr = GetWorld()->GetPlayerControllerIterator(); PlayerItr; ++PlayerItr)
		{
			ABasePlayerController * PC = Cast<ABasePlayerController>(*PlayerItr);
			if (PC)
			{
				PC->ClientOnRoundOver(RoundWinner);
			}
		}

		// Check if the round winner also won the match
		bool bGameOver = false;
		switch (RoundWinner)
		{
		case (ETeamEnum::TeamA) :
			RoundGameState->TeamARoundsWon++;
			bGameOver = RoundGameState->TeamARoundsWon > NumRoundsPerHalf;
			break;
		case (ETeamEnum::TeamB) :
			RoundGameState->TeamBRoundsWon++;
			bGameOver = RoundGameState->TeamBRoundsWon > NumRoundsPerHalf;
			break;
		}

		if (bGameOver)
		{
			EndMatch(RoundWinner);
			return;
		}

		// Check if half time should be entered
		const int32 NumRounds = RoundGameState->TeamARoundsWon + RoundGameState->TeamBRoundsWon;
		if (NumRounds == NumRoundsPerHalf)
		{
			EnterHalfTime();
			return;
		}
		else if (NumRounds >= NumRoundsPerHalf * 2)
		{
			EndMatch();
			return;
		}
	}

	GetWorldTimerManager().SetTimer(TimerHande_ChangeState, this, &ARoundGameMode::NextRound, 5.f);
}

void ARoundGameMode::NextRound()
{
	ARoundGameState * RoundGameState = Cast<ARoundGameState>(GameState);
	if (RoundGameState)
	{
		RoundGameState->RoundState = ERoundStateEnum::PreRound;
		// Buy time
		RoundGameState->TimeRemaining = 15;
	}

	ResetLevel();

	for (FConstPlayerControllerIterator PlayerItr = GetWorld()->GetPlayerControllerIterator(); PlayerItr; ++PlayerItr)
	{
		ABasePlayerController * PC = Cast<ABasePlayerController>(*PlayerItr);
		if (PC)
		{
			ASoldier * Soldier = Cast<ASoldier>(PC->GetPawn());
			if (Soldier)
			{
				// Move players who didn't die last round to their spawn location
				AActor * PlayerStart = FindPlayerStart(PC);
				if (PlayerStart)
				{
					if (Soldier->IsDead())
					{
						PC->UnPossess();
						Soldier->Destroy();
						PlayerSpawn(PC);
					}
					else
					{
						Soldier->SetActorLocationAndRotation(PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());
					}
				}
			}
			else
			{
				// Respawn dead players
				PlayerSpawn(PC);
			}

			Soldier = Cast<ASoldier>(PC->GetPawn());
			if (Soldier)
			{
				// Disable movement for all players
				Soldier->GetCharacterMovement()->StopMovementImmediately();
				Soldier->GetCharacterMovement()->Deactivate();
			}
		}
	}
}

void ARoundGameMode::StartRound()
{
	ARoundGameState * RoundGameState = Cast<ARoundGameState>(GameState);
	if (RoundGameState)
	{
		RoundGameState->RoundState = ERoundStateEnum::InProgress;
		RoundGameState->TimeRemaining = RoundLength;
	}

	for (FConstPlayerControllerIterator PlayerItr = GetWorld()->GetPlayerControllerIterator(); PlayerItr; ++PlayerItr)
	{
		ABasePlayerController * PC = Cast<ABasePlayerController>(*PlayerItr);
		if (PC)
		{
			ASoldier * Soldier = Cast<ASoldier>(PC->GetPawn());
			if (Soldier)
			{
				// Reenable movement for all players
				Soldier->GetCharacterMovement()->Activate();
			}
		}
	}
}

void ARoundGameMode::OnRoundTimeExpired()
{
	// By default noone wins when round time expires
	EndRound(ETeamEnum::None);
}

void ARoundGameMode::EnterHalfTime()
{
	SetMatchState(MatchState::WaitingPostMatch);
	MoveAllPlayersToState(EPlayerStateEnum::MatchEnd);
	GetWorldTimerManager().SetTimer(TimerHande_ChangeState, this, &ARoundGameMode::ExitHalfTime, 10.f);
}

void ARoundGameMode::ExitHalfTime()
{
	ARoundGameState * RoundGameState = Cast<ARoundGameState>(GameState);
	if (RoundGameState)
	{
		int32 TeamARounds = RoundGameState->TeamARoundsWon;
		RoundGameState->TeamARoundsWon = RoundGameState->TeamBRoundsWon;
		RoundGameState->TeamBRoundsWon = TeamARounds;
	}

	for (FConstPlayerControllerIterator PlayerItr = GetWorld()->GetPlayerControllerIterator(); PlayerItr; ++PlayerItr)
	{
		ABasePlayerController * PC = Cast<ABasePlayerController>(*PlayerItr);
		if (PC)
		{
			// Move all players to the opposite team
			PC->SetTeam(PC->GetTeam() == ETeamEnum::TeamA ? ETeamEnum::TeamB : ETeamEnum::TeamA);
		}
	}

	NextRound();
}

void ARoundGameMode::OnPlayerDied(ABasePlayerController * PC)
{
	Super::OnPlayerDied(PC);

	if (GetRoundState() == ERoundStateEnum::WarmUp || GetRoundState() == ERoundStateEnum::RoundEnd) return;

	for (TActorIterator<ASoldier> SoldierItr(GetWorld()); SoldierItr; ++SoldierItr)
	{
		if (!SoldierItr->IsDead() && SoldierItr->GetTeam() == PC->GetTeam())
		{
			// If any soldier of same team is still alive, don't end round
			return;
		}
	}

	EndRound(PC->GetTeam() == ETeamEnum::TeamA ? ETeamEnum::TeamB : ETeamEnum::TeamA);
}