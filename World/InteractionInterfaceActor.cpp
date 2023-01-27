#include "FPSTemplate.h"
#include "InteractionInterfaceActor.h"

bool AInteractionInterfaceActor::CanSoldierInteract(ASoldier * Soldier) const
{
	return K2_CanSoldierInteract(Soldier);
}

float AInteractionInterfaceActor::GetInteractionLength() const
{
	return K2_GetInteractionLength();
}

FVector AInteractionInterfaceActor::GetInteractionLocation() const
{
	return K2_GetInteractionLocation();
}

UAnimMontage * AInteractionInterfaceActor::GetInteractionMontage(ASoldier * Soldier) const
{
	return K2_GetInteractionMontage(Soldier);
}

void AInteractionInterfaceActor::OnInteract(ASoldier * Soldier)
{
	K2_OnInteract(Soldier);
}