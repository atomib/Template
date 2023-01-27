#pragma once

#include "Bomb/Bomb.h"
#include "Game/RoundGameState.h"
#include "BombDefuseGameState.generated.h"

UCLASS()
class FPSTEMPLATE_API ABombDefuseGameState : public ARoundGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = Bomb)
		ABomb * Bomb;
	
	
};
