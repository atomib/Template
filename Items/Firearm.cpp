#include "FPSTemplate.h"
#include "Firearm.h"
#include "UnrealNetwork.h"
#include "Soldier/Soldier.h"
#include "StaticFunctionLibrary.h"
#include "BaseGameInstance.h"

AFirearm::AFirearm(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	SuppressorComponent = ObjectInitializer.CreateOptionalDefaultSubobject<USuppressorComponent>(this, TEXT("SuppressorComponent"));
	SuppressorComponent->SetupAttachment(Mesh, FName(TEXT("Muzzle")));

	static ConstructorHelpers::FObjectFinder<UTexture2D> NewCrosshairTexture(TEXT("Texture2D'/Game/FPSTemplate/HUD/Textures/T_Crosshair.T_Crosshair'"));
	CrosshairTexture = NewCrosshairTexture.Object;

	bChamber = true;
	Magazine = -1;
	MagazineType = EMagazineTypeEnum::Default;
}

void AFirearm::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFirearm, bAuto);
	DOREPLIFETIME(AFirearm, bChamber);
	DOREPLIFETIME(AFirearm, Magazine);
	DOREPLIFETIME(AFirearm, Ammunition);
	DOREPLIFETIME(AFirearm, ProjectileClass);
}

void AFirearm::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	// If a auto fire mode exists, it should be selected by default
	if (bHasAutoFireMode)
	{
		bAuto = true;
	}

	// Set magazine to magazine capacity
	Magazine = MagazineCapacity;

	// Fill optional reload long animation variables
	if (!ReloadLongAnimation.Sequence)
	{
		ReloadLongAnimation.Sequence = ReloadAnimation.Sequence;
	}
	if (!ReloadLongAnimation.CharacterSequence)
	{
		ReloadLongAnimation.CharacterSequence = ReloadAnimation.CharacterSequence;
	}
	if (ReloadLongAnimation.Length == 0.f)
	{
		ReloadLongAnimation.Length = ReloadAnimation.Length;
	}
}

void AFirearm::BeginPlay()
{
	Super::BeginPlay();

	// Destroy suppressor component if it is not needed
	if (SuppressorComponent && !SuppressorComponent->GetVisibleFlag())
	{
		SuppressorComponent->DestroyComponent();
	}
}

void AFirearm::Reset()
{
	Super::Reset();

	Magazine = MagazineCapacity;
	Ammunition = GetDefault<AFirearm>(GetClass())->Ammunition;
	bChamber = true;
}

void AFirearm::SetLoadout(const struct FLoadoutSlotStruct & LoadoutSlot)
{
	Super::SetLoadout(LoadoutSlot);

	UBaseGameInstance * GameInstance = UStaticFunctionLibrary::GetBaseGameInstance(this);
	if (!GameInstance) return;

	TSubclassOf<ABaseProjectile> NewProjectileClass = GameInstance->GetProjectile(LoadoutSlot.ProjectileClassName);
	if (*NewProjectileClass)
	{
		ProjectileClass = NewProjectileClass;
	}

	if (SuppressorComponent && LoadoutSlot.bHasSuppressor)
	{
		SuppressorComponent->Equip();
	}
}

float AFirearm::GetImprecision(float AimRatio, float BulletCount, const FVector & SoldierVelocity) const
{
	// Return imprecision based on AimRatio, current BulletCount and the owning soldiers velocity
	return ((RecoilCurve ? RecoilCurve->GetVectorValue(BulletCount).Z : 1.f) + SoldierVelocity.Size() * 0.001f) * (6.f - AimRatio * 5.f);
}

void AFirearm::DrawItemHUD(APlayerController * PC, UCanvas * Canvas, FVector CameraPosition, FVector CameraDir, float AimRatio, float BulletCount, const FVector & SoldierVelocity)
{
	// Draw typical firearm crosshair
	if (CrosshairTexture)
	{
		const float CrosshairOffset = GetImprecision(AimRatio, BulletCount, SoldierVelocity) * 20.f;
		const float CrosshairOpacity = 1.f - AimRatio;
		for (uint8 i = 0; i < 4; i++)
		{
			float MultiX = 0.f;
			float MultiY = -1.f;

			switch (i)
			{
			case (1) :
				MultiX = 1.f;
				MultiY = 0.f;
				break;
			case (2) :
				MultiX = 0.f;
				MultiY = 1.f;
				break;
			case (3) :
				MultiX = -1.f;
				MultiY = 0.f;
				break;
			}

			const FVector2D ScreenPosition = FVector2D(Canvas->SizeX * 0.5f - 3.f, Canvas->SizeY * 0.5f - 24.f) + FVector2D(MultiX * CrosshairOffset, MultiY * CrosshairOffset);
			Canvas->K2_DrawTexture(CrosshairTexture, ScreenPosition, FVector2D(CrosshairTexture->GetSizeX(), CrosshairTexture->GetSizeY()), FVector2D::ZeroVector, FVector2D::UnitVector, FLinearColor(1.f, 1.f, 1.f, CrosshairOpacity), EBlendMode::BLEND_Translucent, i * 90.f, FVector2D(0.5f, 1.f));
		}
	}
}

int32 AFirearm::GetMagazine() const
{
	return Magazine;
}

int32 AFirearm::GetAmmunition() const
{
	return Ammunition;
}

bool AFirearm::GetIsAuto() const
{
	return bAuto;
}

bool AFirearm::HasFullAmmo() const
{
	return Ammunition == GetDefault<AFirearm>(GetClass())->Ammunition;
}

void AFirearm::RefillAmmo()
{
	Ammunition = GetDefault<AFirearm>(GetClass())->Ammunition;
}

void AFirearm::StartFiring()
{
	Fire();
}

void AFirearm::StopFiring()
{

}

void AFirearm::ToggleFireMode()
{
	if (bAuto)
	{
		// If current fire mode is auto and a semi mode exists, toggle
		if (bHasSemiFireMode)
		{
			bAuto = false;
		}
	}
	else
	{
		// If fire mode is semi and a auto fire mode exists, toggle
		if (bHasAutoFireMode)
		{
			bAuto = true;
		}
	}
}

void AFirearm::Fire()
{
	if (bChamber)
	{
		if (PerformTask(EItemTaskEnum::Fire, FireAnimation.Length))
		{
			bChamber = false;
			OnFire();

			if (Magazine > 0 && (bAuto || bHasSemiFireMode))
			{
				// Instantly reload chamber if item is semi or full auto
				bChamber = true;
				Magazine--;
			}
		}
	}
}

void AFirearm::Reload()
{
	if (Magazine < MagazineCapacity && Ammunition > 0)
	{
		if (bChamber)
		{
			if (PerformTask(EItemTaskEnum::Reload, ReloadAnimation.Length))
			{
				OnReload();
			}
		}
		else
		{
			ReloadLong();
		}
	}
}

void AFirearm::ReloadLong()
{
	if (PerformTask(EItemTaskEnum::ReloadLong, ReloadLongAnimation.Length))
	{
		OnReloadLong();
	}
}

void AFirearm::ReloadChamber()
{
	if (!bChamber && Magazine > 0 && PerformTask(EItemTaskEnum::ReloadChamber, ReloadChamberAnimation.Length))
	{
		OnReloadChamber();
	}
}

void AFirearm::OnDrew()
{
	if (!bChamber)
	{
		if (Magazine > 0)
		{
			ReloadChamber();
		}
		else
		{
			Reload();
		}
	}
	else if (IsSoldierFiring())
	{
		// Start firing if soldiers is holding down fire
		Fire();
	}
}

void AFirearm::OnFired()
{
	if (!bChamber)
	{
		if (Magazine > 0)
		{
			// Chamber can be reloaded
			ReloadChamber();
		}
		else
		{
			// Reload if magazine is empty
			Reload();
		}
	}
	else if (bAuto && IsSoldierFiring())
	{
		Fire();
	}
}

void AFirearm::OnReloaded()
{
	if (MagazineType == EMagazineTypeEnum::Default)
	{
		// Number of rounds needed to fill magazine
		const uint16 RoundsNeeded = MagazineCapacity - Magazine;
		if (Ammunition >= RoundsNeeded)
		{
			// If enough ammunition exists, fill magazine and subtract ammunition
			Magazine = MagazineCapacity;
			Ammunition -= RoundsNeeded;
		}
		else
		{
			// Else fill magazine and empty ammunition
			Magazine += Ammunition;
			Ammunition = 0;
		}

		if (IsSoldierFiring())
		{
			Fire();
		}
	}
	else
	{
		// Reload single bullet
		Magazine++;
		Ammunition--;

		if (IsSoldierFiring())
		{
			Fire();
		}
		else
		{
			// Continue reload if magazine is not full yet
			Reload();
		}
	}
}

void AFirearm::OnReloadedLong()
{
	// Perform OnReloaded logic
	OnReloaded();
	// Finish reload by reloading the chamber
	ReloadChamber();
}

void AFirearm::OnReloadedChamber()
{
	bChamber = true;
	Magazine--;

	if (MagazineType != EMagazineTypeEnum::Tubular && IsSoldierFiring())
	{
		// Do not continue firing when item has a tubular magazin, otherwise it would continue firing without having to reset the trigger
		Fire();
	}
}

void AFirearm::OnFire()
{
	if (GetNetMode() == NM_DedicatedServer) return;

	ASoldier * Soldier = GetOwningSoldier();
	if (Soldier)
	{
		Soldier->FireProjectile(ProjectileClass, RecoilCurve, VisualRecoil);
	}

	PlayAnimation(FireAnimation, 0.02f, 0.02f);

	const FVector MuzzleLocation = Mesh->GetSocketLocation(FName(TEXT("Muzzle")));
	// Fire sound
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, MuzzleLocation);
	// Muzzle flash
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, FName(TEXT("Muzzle")), SuppressorComponent && SuppressorComponent->bEquipped ? FVector(SuppressorComponent->Length, 0.f, 0.f) : FVector::ZeroVector);

	// Set timer on clients for auto fire
	if (GetNetMode() == NM_Client && bAuto)
	{
		Soldier->OnClientStartAutoFire(FireAnimation.Length);
	}
}

void AFirearm::OnReload()
{
	ASoldier * Soldier = GetOwningSoldier();
	if (Soldier)
	{
		PlayAnimation(ReloadAnimation, 0.25f, 0.25f, Soldier->HasPerk(EPerkEnum::FastReload) ? FastAnimationRate : 1.f);
	}
}

void AFirearm::OnReloadLong()
{
	ASoldier * Soldier = GetOwningSoldier();
	if (Soldier)
	{
		PlayAnimation(ReloadLongAnimation, 0.25f, 0.25f, Soldier->HasPerk(EPerkEnum::FastReload) ? FastAnimationRate : 1.f);
	}
}

void AFirearm::OnReloadChamber()
{
	ASoldier * Soldier = GetOwningSoldier();
	if (Soldier)
	{
		PlayAnimation(ReloadChamberAnimation, 0.25f, 0.25f, Soldier->HasPerk(EPerkEnum::FastReload) ? FastAnimationRate : 1.f);
	}
}