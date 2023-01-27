#pragma once

#include "Game/FPSGameState.h"
#include "TicketGameState.generated.h"

UCLASS()
class FPSTEMPLATE_API ATicketGameState : public AFPSGameState
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
		int32 MaxTickets;

	UPROPERTY(Replicated)
		int32 TeamATickets;

	UPROPERTY(Replicated)
		int32 TeamBTickets;

public:
	void SetMaxTickets(uint16 Val);

	void SetStartTickets(uint16 Val);

	UFUNCTION(BlueprintPure, Category = GameState)
		int32 GetMaxTickets() const { return MaxTickets; };

	// Add tickets to team A ticket count and return whether new value is out of range
	bool AddTeamATickets(int32 Val);

	UFUNCTION(BlueprintPure, Category = GameState)
		int32 GetTeamATickets() const { return TeamATickets; };

	// Add tickets to team B ticket count and return whether new value is out of range
	bool AddTeamBTickets(int32 Val);

	UFUNCTION(BlueprintPure, Category = GameState)
		int32 GetTeamBTickets() const { return TeamBTickets; };
	
};
