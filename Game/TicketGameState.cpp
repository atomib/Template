#include "FPSTemplate.h"
#include "TicketGameState.h"
#include "UnrealNetwork.h"

void ATicketGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ATicketGameState, MaxTickets, COND_InitialOnly);
	DOREPLIFETIME(ATicketGameState, TeamATickets);
	DOREPLIFETIME(ATicketGameState, TeamBTickets);
}

void ATicketGameState::SetMaxTickets(uint16 Val)
{
	MaxTickets = Val;
}

void ATicketGameState::SetStartTickets(uint16 Val)
{
	TeamATickets = Val;
	TeamBTickets = Val;
	SetMaxTickets(Val);
}

bool ATicketGameState::AddTeamATickets(int32 Val)
{
	TeamATickets = FMath::Clamp(TeamATickets + Val, 0, MaxTickets);
	return TeamATickets == 0 || TeamATickets == MaxTickets;
}

bool ATicketGameState::AddTeamBTickets(int32 Val)
{
	TeamBTickets = FMath::Clamp(TeamBTickets + Val, 0, MaxTickets);
	return TeamBTickets == 0 || TeamBTickets == MaxTickets;
}