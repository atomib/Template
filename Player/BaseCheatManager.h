#pragma once

#include "GameFramework/CheatManager.h"
#include "BaseCheatManager.generated.h"

// Cheat manager alows you to perform cheats through the console
UCLASS()
class FPSTEMPLATE_API UBaseCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
	// Gives the soldier an item of specified type (name of blueprint class, must be listed in BP_ItemLibrary and end in "_C")
	UFUNCTION(Exec)
		void GiveWeapon(const FName & ClassName);

	// Same as GiveWeapon, but for all players
	UFUNCTION(Exec)
		void GiveWeaponToAll(const FName & ClassName);

	// Apply damage to soldier
	UFUNCTION(Exec)
		void DamageSoldier(float DamageAmount);
	
	
};
