#include "FPSTemplate.h"
#include "ThrowableItem.h"
#include "Soldier/Soldier.h"

AThrowableItem::AThrowableItem(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	Ammunition = 1;
}

void AThrowableItem::StartFiring()
{
	if (Ammunition != 0 && PerformTask(EItemTaskEnum::Fire, ThrowAnimation.Length))
	{
		OnFire();
	}
}

void AThrowableItem::OnFired()
{
	ASoldier * Soldier = GetOwningSoldier();
	if (Soldier)
	{
		Soldier->ThrowProjectile(ProjectileClass);
		Ammunition--;

		if (Ammunition == 0)
		{
			// Destroy this item of no ammunition is left
			Soldier->DropItem();
			Destroy();
		}
	}
}

void AThrowableItem::OnFire()
{
	PlayAnimation(ThrowAnimation);
}