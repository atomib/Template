#pragma once

#include "Attachments/Sight.h"
#include "Components/StaticMeshComponent.h"
#include "SightComponent.generated.h"

UCLASS(ClassGroup = (Attachments), meta = (BlueprintSpawnableComponent))
class FPSTEMPLATE_API USightComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	// Animations sequence to use when the player aims the owning item with a sight
	UPROPERTY(EditDefaultsOnly, Category = Sight)
		UAnimSequenceBase * CharacterAimSequence;

	// Class of sight to use properties from
	UPROPERTY(ReplicatedUsing = OnRep_SightClass)
		TSubclassOf<USight> SightClass;

private:
	UFUNCTION()
		void OnRep_SightClass();

public:
	USightComponent(const FObjectInitializer & ObjectInitializer);

	// Only works initially!
	void SetSightClass(TSubclassOf<USight> NewSightClass);
	
	
};
