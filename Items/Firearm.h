#pragma once

#include "Attachments/SuppressorComponent.h"
#include "Projectiles/BaseProjectile.h"
#include "Items/Item.h"
#include "Firearm.generated.h"

UENUM()
enum class EMagazineTypeEnum : uint8
{
	Default,
	Tubular,
};

// Base class for all firearms. For non automatic weapons (e.g. bolt action sniper rifles) set bSemi and bAuto to false
UCLASS()
class FPSTEMPLATE_API AFirearm : public AItem
{
	GENERATED_BODY()

public:
	// Suppressor component will be deleted if not set to visible
	UPROPERTY(EditAnywhere, Category = Components)
		USuppressorComponent * SuppressorComponent;

	// Indicates if this gun has a semi automatic fire mode
	UPROPERTY(EditDefaultsOnly, Category = Firearm)
		bool bHasSemiFireMode;

	// Whether automatic fire mode exists
	UPROPERTY(EditDefaultsOnly, Category = Firearm)
		bool bHasAutoFireMode;

	// Whether a full magazine will be reloaded or a single shot (e.g. for shotguns)
	UPROPERTY(EditDefaultsOnly, Category = Firearm)
		EMagazineTypeEnum MagazineType;

protected:
	// Whether auto firemode is enabled, replicated
	UPROPERTY(Replicated)
		bool bAuto;

	// Whether bullet is loaded into chamber, replicated
	UPROPERTY(Replicated)
		bool bChamber;

	// Rounds currently in magazine, replicated
	UPROPERTY(Replicated)
		uint16 Magazine;

public:
	UPROPERTY(Replicated, EditDefaultsOnly, Category = Firearm)
		TSubclassOf<ABaseProjectile> ProjectileClass;

	// Name of cartridge which this firearm is compatible with
	/*UPROPERTY(Replicated, EditDefaultsOnly, Category = Firearm)
		FName CartridgeName;*/

	// Vector curve used for recoil and accuracy. X = vertical recoil, Y = horizontal recoil, Z = imprecision
	UPROPERTY(EditDefaultsOnly, Category = Firearm)
		UCurveVector * RecoilCurve;

	// Offset applied to weapon when firing
	UPROPERTY(EditDefaultsOnly, Category = Firearm)
		FVector VisualRecoil;

	// Particle spawned at bone called "muzzle"
	UPROPERTY(EditDefaultsOnly, Category = Firearm)
		UParticleSystem * MuzzleFlash;

	// Sound cue played when firing
	UPROPERTY(EditDefaultsOnly, Category = Firearm)
		USoundCue * FireSound;

	// Ammunition for this item, replicated
	UPROPERTY(Replicated, EditDefaultsOnly, Category = Firearm)
		int32 Ammunition;

	// Magazine capacity (without chamber)
	UPROPERTY(EditDefaultsOnly, Category = Firearm)
		int32 MagazineCapacity;

	// Animations
	UPROPERTY(EditDefaultsOnly, Category = Animations)
		FItemAnimation FireAnimation;

	// TODO
	/*UPROPERTY(EditDefaultsOnly, Category = Animations)
		UAnimSequenceBase * FireAimSequence;

protected:*/
	// Dynamically create montage for fire animation
	//UAnimMontage * CharacterFireMontage;

public:
	UPROPERTY(EditDefaultsOnly, Category = Animations)
		FItemAnimation ReloadAnimation;

	// Optional animation for a long reload, missing values will be taken from ReloadAnimation
	UPROPERTY(EditDefaultsOnly, Category = Animations)
		FItemAnimation ReloadLongAnimation;

	UPROPERTY(EditDefaultsOnly, Category = Animations)
		FItemAnimation ReloadChamberAnimation;
	// End of animations

	// Constructor
	AFirearm(const FObjectInitializer & ObjectInitializer);

	// Called after spawn
	virtual void OnConstruction(const FTransform & Transform) override;

	virtual void BeginPlay() override;

	virtual void Reset() override;

	virtual void SetLoadout(const struct FLoadoutSlotStruct & LoadoutSlot) override;

	float GetImprecision(float AimRatio, float BulletCount, const FVector & SoldierVelocity) const override;

	virtual void DrawItemHUD(APlayerController * PC, UCanvas * Canvas, FVector CameraPosition, FVector CameraDir, float AimRatio, float BulletCount, const FVector & SoldierVelocity) override;

	virtual int32 GetMagazine() const override;

	virtual int32 GetAmmunition() const override;

	virtual bool GetIsAuto() const override;

	virtual bool HasFullAmmo() const override;

	virtual void RefillAmmo() override;

	virtual void StartFiring() override;

	virtual void StopFiring() override;

	virtual void ToggleFireMode() override;

	// Tasks
	virtual void Fire() override;

	virtual void Reload() override;

protected:
	virtual void ReloadLong() override;

	virtual void ReloadChamber() override;

	// Task notifies
	virtual void OnDrew() override;

	virtual void OnFired() override;

	virtual void OnReloaded() override;

	virtual void OnReloadedLong() override;

	virtual void OnReloadedChamber() override;

	// Task events
public:
	virtual void OnFire() override;

protected:
	virtual void OnReload() override;

	virtual void OnReloadLong() override;

	virtual void OnReloadChamber() override;
	
};
