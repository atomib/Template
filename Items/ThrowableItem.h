#pragma once

#include "Projectiles/ThrowableProjectile.h"
#include "Items/Item.h"
#include "ThrowableItem.generated.h"

UCLASS()
class FPSTEMPLATE_API AThrowableItem : public AItem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = Throwable)
		TSubclassOf<AThrowableProjectile> ProjectileClass;

	// Amount of projectiles which can be thrown, negative values indicate infinite ammo
	UPROPERTY(EditDefaultsOnly, Category = Throwable)
		int32 Ammunition;

	UPROPERTY(EditDefaultsOnly, Category = Animations)
		FItemAnimation ThrowAnimation;

	AThrowableItem(const FObjectInitializer & ObjectInitializer);

	virtual void StartFiring() override;

	virtual void OnFired() override;

	virtual void OnFire() override;
	
};
