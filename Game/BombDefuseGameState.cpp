#include "FPSTemplate.h"
#include "BombDefuseGameState.h"
#include "UnrealNetwork.h"

void ABombDefuseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABombDefuseGameState, Bomb);
}