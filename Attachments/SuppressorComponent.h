#pragma once

#include "Components/StaticMeshComponent.h"
#include "SuppressorComponent.generated.h"

UCLASS(ClassGroup = (Attachments), meta = (BlueprintSpawnableComponent))
class FPSTEMPLATE_API USuppressorComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	// Particle system and sound used when weapon has a silencer
	UPROPERTY(EditDefaultsOnly, Category = Suppressor)
		UParticleSystem * MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, Category = Suppressor)
		USoundCue * FireSound;

	// Length of this suppressor, or distance between muzzle socket and muzzle of suppressor
	UPROPERTY(EditDefaultsOnly, Category = Suppressor)
		float Length;

	// Whether supressor is equipped or not
	UPROPERTY(ReplicatedUsing = OnRep_bEquipped)
		bool bEquipped;

private:
	UFUNCTION()
		void OnRep_bEquipped();

public:
	USuppressorComponent();
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	// Called from item, enables the suppressor
	void Equip();
	
};
