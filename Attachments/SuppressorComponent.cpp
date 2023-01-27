#include "FPSTemplate.h"
#include "SuppressorComponent.h"
#include "UnrealNetwork.h"
#include "Items/Firearm.h"

USuppressorComponent::USuppressorComponent()
{
	SetCollisionProfileName(FName(TEXT("NoCollision")));
	SetIsReplicated(true);
	SetVisibleFlag(false);
	bHiddenInGame = true;
}

void USuppressorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION(USuppressorComponent, bEquipped, COND_InitialOnly);
}

void USuppressorComponent::Equip()
{
	bEquipped = true;
	OnRep_bEquipped();
}

void USuppressorComponent::OnRep_bEquipped()
{
	SetHiddenInGame(false);

	AFirearm * OwnerItem = Cast<AFirearm>(GetOuter());
	if (OwnerItem)
	{
		// Update fire effects on owning item
		OwnerItem->MuzzleFlash = MuzzleFlash;
		OwnerItem->FireSound = FireSound;
	}
}