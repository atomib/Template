#pragma once

#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

UENUM(BlueprintType)
enum class ETeamEnum : uint8
{
	None,
	TeamA,
	TeamB,
	Max,
};

UCLASS()
class FPSTEMPLATE_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
		uint16 Kills;

	UPROPERTY(Replicated)
		uint16 Deaths;

	UPROPERTY(Replicated)
		ETeamEnum Team;

	UFUNCTION(BlueprintPure, Category = PlayerState)
		int32 GetKills() const { return Kills; };

	UFUNCTION(BlueprintPure, Category = PlayerState)
		int32 GetDeaths() const { return Deaths; };

	UFUNCTION(BlueprintPure, Category = PlayerState)
		int32 GetScore() const { return Score; };

	UFUNCTION(BlueprintPure, Category = PlayerState)
		ETeamEnum GetTeam() const { return Team; };

	UFUNCTION(BlueprintPure, Category = PlayerState)
		bool IsEnemyFor(ABasePlayerState * OtherPlayer) const;

	void SetTeam(ETeamEnum NewTeam);
	
};
