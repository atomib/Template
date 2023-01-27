#include "FPSTemplate.h"
#include "CustomizationScreen.h"
#include "StaticFunctionLibrary.h"
#include "Items/Item.h"
#include "Items/Firearm.h"

ACustomizationScreen::ACustomizationScreen(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	FName NoCollisionName = FName("NoCollision");

	BackgroundMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("BackgroundMesh"));
	RootComponent = BackgroundMesh;
	BackgroundMesh->SetCollisionProfileName(NoCollisionName);

	ItemMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(BackgroundMesh);

	Sight = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Sight"));
	Sight->SetupAttachment(ItemMesh, FName("b_gun_Root"));
	Sight->SetCollisionProfileName(NoCollisionName);

	Suppressor = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Suppressor"));
	Suppressor->SetupAttachment(ItemMesh, FName("Muzzle"));
	Suppressor->SetCollisionProfileName(NoCollisionName);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bAllowTickOnDedicatedServer = false;
}

void ACustomizationScreen::BecomeViewTarget(APlayerController * PC)
{
	EnableInput(PC);
	SetActorTickEnabled(true);
}

void ACustomizationScreen::EndViewTarget(APlayerController * PC)
{
	DisableInput(PC);
	SetActorTickEnabled(true);
}

void ACustomizationScreen::GetItemBounds(FVector & OutOrigin, float & OutSphereRadius) const
{
	if (ItemMesh->SkeletalMesh)
	{
		const FBoxSphereBounds MeshBounds = ItemMesh->SkeletalMesh->GetBounds();
		OutOrigin = MeshBounds.Origin;
		OutSphereRadius = MeshBounds.SphereRadius;
	}
}

void ACustomizationScreen::UpdatePreview(const FLoadoutSlotStruct & LoadoutSlot)
{
	UBaseGameInstance * GameInstance = UStaticFunctionLibrary::GetBaseGameInstance(this);
	if (!GameInstance) return;

	ItemMesh->SetSkeletalMesh(NULL);
	Sight->SetStaticMesh(NULL);
	Suppressor->SetStaticMesh(NULL);

	ItemMesh->OverrideMaterials.Empty();

	AItem * Item = GameInstance->GetItem(LoadoutSlot.ItemClassName).GetDefaultObject();
	if (Item)
	{
		ItemMesh->SetSkeletalMesh(Item->Mesh->SkeletalMesh);
		ItemMesh->SetAnimInstanceClass(Item->Mesh->AnimClass);

		if (Item->Mesh->PhysicsAssetOverride)
		{
			ItemMesh->SetPhysicsAsset(Item->Mesh->PhysicsAssetOverride);
		}

		// Sight
		if (Item->SightComponent)
		{
			Sight->SetRelativeTransform(Item->SightComponent->GetRelativeTransform());

			USight * SightCDO = GameInstance->GetSight(LoadoutSlot.SightClassName).GetDefaultObject();
			if (SightCDO && SightCDO->Mesh)
			{
				Sight->SetStaticMesh(SightCDO->Mesh);
			}
			else
			{
				Sight->SetStaticMesh(Item->SightComponent->GetStaticMesh());
			}
		}

		// Suppressor
		AFirearm * Firearm = Cast<AFirearm>(Item);
		if (LoadoutSlot.bHasSuppressor && Firearm && Firearm->SuppressorComponent)
		{
			Suppressor->SetRelativeTransform(Firearm->SuppressorComponent->GetRelativeTransform());
			Suppressor->SetStaticMesh(Firearm->SuppressorComponent->GetStaticMesh());
		}

		// Get pattern material and assign it
		bool bFoundMaterial = false;
		if (LoadoutSlot.PatternMaterialName != NAME_None)
		{
			UMaterialInterface * PatternMaterial = GameInstance->GetItemPatternMaterial(Item->GetClass(), LoadoutSlot.PatternMaterialName);
			if (PatternMaterial)
			{
				ItemMesh->SetMaterial(Item->PatternMaterialSlot, PatternMaterial);
				bFoundMaterial = true;
			}
		}
	}
}