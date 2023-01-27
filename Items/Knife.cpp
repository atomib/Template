#include "FPSTemplate.h"
#include "Knife.h"
#include "Soldier/Soldier.h"
#include "BaseSingleton.h"

bool AKnife::CanBeDropped() const
{
	// A knife may never be dropped to acoid that players always have at least one item
	return false;
}

void AKnife::StartFiring()
{
	Fire();
}

void AKnife::Fire()
{
	if (PerformTask(EItemTaskEnum::Fire, StabAnimation.Length))
	{

		OnFire();
	}
}

void AKnife::OnFire()
{
	PlayAnimation(StabAnimation, 0.1f);

	ASoldier * Soldier = GetOwningSoldier();
	if (Soldier)
	{
		FHitResult Hit;
		// Execute stab logic on soldier
		if (Soldier->Stab(Hit, StabDamage, StabDistance))
		{
			SpawnImpactEffect(Hit);
		}
	}
}

void AKnife::SpawnImpactEffect(const FHitResult & HitResult)
{
	USoundBase * ImpactSound;
	UParticleSystem * ImpactEmitter;
	UMaterialInterface * DecalMaterial;

	UBaseSingleton::GetKnifeImpactAssets(UGameplayStatics::GetSurfaceType(HitResult), ImpactSound, ImpactEmitter, DecalMaterial);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEmitter, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, HitResult.ImpactPoint);
	UDecalComponent * Decal = UGameplayStatics::SpawnDecalAttached(DecalMaterial, FVector(10.f, 10.f, 2.f), HitResult.Component.Get(), HitResult.BoneName, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 20.f);
}