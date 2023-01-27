#include "FPSTemplate.h"
#include "FPSGameState.h"
#include "UnrealNetwork.h"
#include "Game/FPSGameMode.h"

void AFPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSGameState, MatchWinner);
	DOREPLIFETIME(AFPSGameState, WinnerPlayerState);
	DOREPLIFETIME(AFPSGameState, TimeRemaining);
}

void AFPSGameState::DefaultTimer()
{
	Super::DefaultTimer();

	if (IsMatchInProgress() && TimeRemaining > 0)
	{
		TimeRemaining--;

		if (TimeRemaining == 0)
		{
			AFPSGameMode * FPSGameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
			if (FPSGameMode)
			{
				// Notify game mode that match timed out
				FPSGameMode->OnTimeExpired();
			}
		}
	}
}