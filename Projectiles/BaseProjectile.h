#pragma once

#include "Projectiles/BaseProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class FPSTEMPLATE_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

	// Cached distance to the local player, used for bullet crack
	float ListenerDistSquared;

public:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		UBaseProjectileMovementComponent * ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		USphereComponent * SphereCollision;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		UStaticMeshComponent * Mesh;

	// Cartidge compatibility
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		FName CompatibilityTag;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		USoundBase * FlybySound;

	ABaseProjectile(const FObjectInitializer & ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void OnProjectileStop(const FHitResult & HitResult);

	UFUNCTION()
		virtual void OnProjectileHit(const FHitResult & HitResult, const FVector & ImpactVelocity);

	virtual void SpawnImpactEffect(const FHitResult & HitResult);
	
};
