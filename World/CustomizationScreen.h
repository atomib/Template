#pragma once

#include "Game/LoadoutBuilderComponent.h"
#include "GameFramework/Actor.h"
#include "CustomizationScreen.generated.h"

// Blueprint class derived from this can be found under Blueprints/BP_CustomizationScreen
UCLASS()
class FPSTEMPLATE_API ACustomizationScreen : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
		UStaticMeshComponent * Sight;

	UPROPERTY()
		UStaticMeshComponent * Suppressor;

public:
	UPROPERTY(EditAnywhere, Category = Defaults)
		UStaticMeshComponent * BackgroundMesh;

	UPROPERTY(EditAnywhere, Category = Defaults)
		USkeletalMeshComponent * ItemMesh;
	
	ACustomizationScreen(const FObjectInitializer & ObjectInitializer);

	void BecomeViewTarget(APlayerController * PC) override;

	void EndViewTarget(APlayerController * PC) override;

	// Update ItemMesh and the pattern material
	UFUNCTION(BlueprintCallable, Category = Defaults)
		void UpdatePreview(UPARAM(ref) const FLoadoutSlotStruct & LoadoutSlot);

	UFUNCTION(BlueprintPure, Category = Defaults)
		void GetItemBounds(FVector & OutOrigin, float & OutSphereRadius) const;

	/*UFUNCTION(BlueprintImplementableEvent, Category = Defaults)
		void OnUpdatePreview();*/
	
};
