#include "FPSTemplate.h"
#include "FreeForAllGameState.h"
#include "UnrealNetwork.h"

AFreeForAllGameState::AFreeForAllGameState()
{
	MaxKills = 50;
}

void AFreeForAllGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFreeForAllGameState, MaxKills, COND_InitialOnly);
}

void AFreeForAllGameState::SetMaxKills(int32 Val)
{
	MaxKills = Val;
}