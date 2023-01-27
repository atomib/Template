#pragma once

#include "GameFramework/DamageType.h"
#include "BaseDamageType.generated.h"

UCLASS()
class FPSTEMPLATE_API UBaseDamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	// Icon shown in kill feed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DamageType)
		UTexture2D * Icon;
	
	// How many points a player will get for killing with this damage type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DamageType)
		int32 KillReward;

};
