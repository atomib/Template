#pragma once

#include "Soldier/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "InteractionInterfaceActor.generated.h"

// Special class to allow blueprints to use the InteractionInterface (for example usage see BP_Elevator)
UCLASS()
class FPSTEMPLATE_API AInteractionInterfaceActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = Interaction)
		bool K2_CanSoldierInteract(ASoldier * Soldier) const;

	UFUNCTION(BlueprintImplementableEvent, Category = Interaction)
		float K2_GetInteractionLength() const;

	UFUNCTION(BlueprintImplementableEvent, Category = Interaction)
		FVector K2_GetInteractionLocation() const;

	UFUNCTION(BlueprintImplementableEvent, Category = Interaction)
		UAnimMontage * K2_GetInteractionMontage(ASoldier * Soldier) const;

	UFUNCTION(BlueprintImplementableEvent, Category = Interaction)
		void K2_OnInteract(ASoldier * Soldier);

	// Interaction interface
	virtual bool CanSoldierInteract(ASoldier * Soldier) const override;

	virtual float GetInteractionLength() const override;

	virtual FVector GetInteractionLocation() const override;

	virtual UAnimMontage * GetInteractionMontage(ASoldier * Soldier) const override;

	virtual void OnInteract(ASoldier * Soldier) override;

	
	
};
