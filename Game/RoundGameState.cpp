#include "FPSTemplate.h"
#include "RoundGameState.h"
#include "UnrealNetwork.h"

void ARoundGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoundGameState, RoundState);
	DOREPLIFETIME(ARoundGameState, TeamARoundsWon);
	DOREPLIFETIME(ARoundGameState, TeamBRoundsWon);
	DOREPLIFETIME_CONDITION(ARoundGameState, NumRoundsPerHalf, COND_InitialOnly);
}