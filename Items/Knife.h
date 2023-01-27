#pragma once

#include "Projectiles/Bullet.h"
#include "Items/Item.h"
#include "Knife.generated.h"

UCLASS()
class FPSTEMPLATE_API AKnife : public AItem
{
	GENERATED_BODY()
	
public:
	// Impact effects
	/*UPROPERTY(EditDefaultsOnly, Category = Effects)
		FImpactEffectStruct MetalEffect;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
		FImpactEffectStruct StoneEffect;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
		FImpactEffectStruct WoodEffect;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
		FImpactEffectStruct FleshEffect;*/

	// Animation used when pressing the fire button
	UPROPERTY(EditDefaultsOnly, Category = Animations)
		FItemAnimation StabAnimation;

	// Damage applied to actors when they get hit
	UPROPERTY(EditDefaultsOnly, Category = Knife)
		float StabDamage;

	// Max distance from soldier camera this knife can damage other actors
	UPROPERTY(EditDefaultsOnly, Category = Knife)
		float StabDistance;

	virtual bool CanBeDropped() const override;

	virtual void StartFiring() override;

	virtual void Fire() override;

	virtual void OnFire() override;

	void SpawnImpactEffect(const FHitResult & HitResult);
	
	
};
