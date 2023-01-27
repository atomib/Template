#include "FPSTemplate.h"
#include "AmmoCrate.h"

AAmmoCrate::AAmmoCrate(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

bool AAmmoCrate::CanSoldierInteract(ASoldier * Soldier) const
{
	return !GetInstigator() && !Soldier->AllItemsFullAmmo(); //(!Instigator || IsActiveItem()) && !Soldier->AllItemsFullAmmo();
}

float AAmmoCrate::GetInteractionLength() const
{
	return 1.f;
}

UAnimMontage * AAmmoCrate::GetInteractionMontage(ASoldier * Soldier) const
{
	return Soldier->SupplyMontage;
}

void AAmmoCrate::OnInteract(ASoldier * Soldier)
{
	Soldier->RefillAmmo(bRefillAllItems);
}