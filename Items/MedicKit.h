#pragma once

#include "Soldier/Soldier.h"
#include "Items/Item.h"
#include "MedicKit.generated.h"

UCLASS()
class FPSTEMPLATE_API AMedicKit : public AItem
{
	GENERATED_BODY()
	
public:
	// HP the soldier will gain after interaction (default player health is 100)
	UPROPERTY(EditDefaultsOnly, Category = MedicKit)
		float HealAmount;

	AMedicKit(const FObjectInitializer & ObjectInitializer);

	virtual bool CanSoldierInteract(ASoldier * Soldier) const override;

	virtual float GetInteractionLength() const override;

	virtual UAnimMontage * GetInteractionMontage(ASoldier * Soldier) const override;

	virtual void OnInteract(ASoldier * Soldier) override;
	
	
};
