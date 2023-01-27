#pragma once

#include "Items/Item.h"
#include "AmmoCrate.generated.h"

UCLASS()
class FPSTEMPLATE_API AAmmoCrate : public AItem
{
	GENERATED_BODY()
	
public:
	// Whether all items of the soldier should be refilled or just one at a time
	UPROPERTY(EditDefaultsOnly, Category = AmmoCrate)
		bool bRefillAllItems;

	AAmmoCrate(const FObjectInitializer & ObjectInitializer);

	virtual bool CanSoldierInteract(ASoldier * Soldier) const override;

	virtual float GetInteractionLength() const override;

	virtual UAnimMontage * GetInteractionMontage(ASoldier * Soldier) const override;

	virtual void OnInteract(ASoldier * Soldier) override;
	
	
};
