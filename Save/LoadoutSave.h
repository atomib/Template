#pragma once

#include "Game/LoadoutBuilderComponent.h"
#include "GameFramework/SaveGame.h"
#include "LoadoutSave.generated.h"

UCLASS()
class FPSTEMPLATE_API ULoadoutSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	// Last loadout slot the player selected
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = Loadout)
		uint8 CurrentSlot;

	UPROPERTY(SaveGame, BlueprintReadOnly, Category = Loadout)
		TArray<FLoadoutStruct> Loadouts;

	ULoadoutSave();

	UFUNCTION(BlueprintCallable, Category = Loadout)
		void SetLoadout(uint8 Slot, const FLoadoutStruct & NewLoadout);

	UFUNCTION(BlueprintPure, Category = Loadout)
		FLoadoutStruct & GetCurrentLoadout();
	
	
};
