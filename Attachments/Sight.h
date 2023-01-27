#pragma once

class UStaticMesh;
class UTexture2D;

#include "UObject/Object.h"
#include "Sight.generated.h"

UCLASS(Blueprintable, BlueprintType)
class FPSTEMPLATE_API USight : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = Sight)
		UStaticMesh * Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sight)
		UTexture2D * SightThumbnail;

	// FOV when zoomed in with this sight, a value of 0 will disable extra zoom
	UPROPERTY(EditDefaultsOnly, Category = Sight)
		float FieldOfView;

	// Distance from the top of the rail to the center of the scope
	UPROPERTY(EditDefaultsOnly, Category = Sight)
		float EyeHeight;

	// Only items which share one or more of their sight components tags with a sights tags can equip this sight
	UPROPERTY(EditDefaultsOnly, Category = Sight)
		TArray<FName> CompatibilityTags;
	
	
};