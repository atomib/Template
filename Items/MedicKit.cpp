#include "FPSTemplate.h"
#include "MedicKit.h"

AMedicKit::AMedicKit(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	HealAmount = 20.f;
}

bool AMedicKit::CanSoldierInteract(ASoldier * Soldier) const
{
	return !GetInstigator() && !Soldier->HasFullHealth(); //(!Instigator || IsActiveItem()) && !Soldier->HasFullHealth();
}

float AMedicKit::GetInteractionLength() const
{
	return 1.f;
}

UAnimMontage * AMedicKit::GetInteractionMontage(ASoldier * Soldier) const
{
	return Soldier->SupplyMontage;
}

void AMedicKit::OnInteract(ASoldier * Soldier)
{
	Soldier->Heal(HealAmount);
}