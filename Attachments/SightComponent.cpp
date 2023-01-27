#include "FPSTemplate.h"
#include "SightComponent.h"
#include "Items/Item.h"
#include "UnrealNetwork.h"

USightComponent::USightComponent(const FObjectInitializer & ObjectInitializer)
{
	SetCollisionProfileName(FName("NoCollision"));
	SetIsReplicated(true);
	SetVisibleFlag(false);
}

void USightComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION(USightComponent, SightClass, COND_InitialOnly);
}

void USightComponent::SetSightClass(TSubclassOf<USight> NewSightClass)
{
	SightClass = NewSightClass;

	if (GetNetMode() != NM_DedicatedServer)
	{
		OnRep_SightClass();
	}
}

void USightComponent::OnRep_SightClass()
{
	AItem * OwnerItem = Cast<AItem>(GetOuter());
	if (OwnerItem)
	{
		const USight * SightCDO = SightClass.GetDefaultObject();
		if (SightCDO)
		{
			SetStaticMesh(SightCDO->Mesh);
			if (CharacterAimSequence)
			{
				// Update aim sequence on owning item
				OwnerItem->SetCharacterAimSequence(CharacterAimSequence);
			}
		}
	}
}