#pragma once

#include "Game/FPSGameState.h"
#include "RoundGameState.generated.h"

UENUM(BlueprintType)
enum class ERoundStateEnum : uint8
{
	WarmUp,
	PreRound,
	InProgress,
	RoundEnd,
};

UCLASS()
class FPSTEMPLATE_API ARoundGameState : public AFPSGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = GameState)
		ERoundStateEnum RoundState;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = GameState)
		int32 TeamARoundsWon;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = GameState)
		int32 TeamBRoundsWon;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = GameState)
		int32 NumRoundsPerHalf;
	
	
};
