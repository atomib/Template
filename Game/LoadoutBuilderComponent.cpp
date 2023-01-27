#include "FPSTemplate.h"
#include "LoadoutBuilderComponent.h"
#include "Items/Item.h"
#include "Items/Firearm.h"
#include "StaticFunctionLibrary.h"
#include "Attachments/Sight.h"
#include "Projectiles/BaseProjectile.h"

#define GET_GAME_INSTANCE() UBaseGameInstance * GameInstance = UStaticFunctionLibrary::GetBaseGameInstance(this); if (!GameInstance) return false;

ULoadoutBuilderComponent::ULoadoutBuilderComponent()
{
	
}

void ULoadoutBuilderComponent::ClearLoadout()
{
	Loadout = FLoadoutStruct();
}

void ULoadoutBuilderComponent::SetLoadoutChecked(const FLoadoutStruct & NewLoadout)
{
	ClearLoadout();
	uint8 OldSlot = CurrentItemSlot;
	CurrentItemSlot = 0;

	EquipPerk(NewLoadout.Perk);

	for (const FLoadoutSlotStruct & Slot : NewLoadout.ItemSlots)
	{
		EquipItem(Slot.ItemClassName);
		EquipPattern(Slot.PatternMaterialName);
		EquipProjectile(Slot.ProjectileClassName);
		EquipSight(Slot.SightClassName);
		EquipSuppressor(Slot.bHasSuppressor);

		for (const FName & Attachment : Slot.AttachmentComponentNames)
		{
			EquipAttachment(Attachment);
		}

		CurrentItemSlot++;
	}

	CurrentItemSlot = OldSlot;
}

FLoadoutSlotStruct & ULoadoutBuilderComponent::GetCurrentSlot()
{
	return Loadout.ItemSlots[CurrentItemSlot];
}

bool ULoadoutBuilderComponent::IsItemFromCurrentSlot(const FName & ItemName) const
{
	GET_GAME_INSTANCE()

	const AItem * ItemCDO = GameInstance->GetItem(ItemName).GetDefaultObject();
	if (ItemCDO)
	{
		// Check if item is in correct slot in loadout
		if (ItemCDO->ItemSlot == EItemSlotEnum::Grenade)
		{
			if (CurrentItemSlot < uint8(EItemSlotEnum::Grenade) - 1)
			{
				return false;
			}
		}
		else if (uint8(ItemCDO->ItemSlot) - 1 != CurrentItemSlot)
		{
			return false;
		}
	}
	else if (CurrentItemSlot == uint8(EItemSlotEnum::Knife) - 1)
	{
		// Do not allow player to equip no knife
		return false;
	}

	return true;
}

bool ULoadoutBuilderComponent::CanEquipItem(const FName & ItemName) const
{
	return IsItemFromCurrentSlot(ItemName);
}

void ULoadoutBuilderComponent::EquipItem(const FName & ItemName)
{
	if (CanEquipItem(ItemName))
	{
		Loadout.ItemSlots[CurrentItemSlot] = FLoadoutSlotStruct(ItemName);
	}
}

bool ULoadoutBuilderComponent::CanEquipPattern(const FName & PatternName) const
{
	return true;
}

void ULoadoutBuilderComponent::EquipPattern(const FName & PatternName)
{
	if (CanEquipPattern(PatternName))
	{
		GetCurrentSlot().PatternMaterialName = PatternName;
	}
}

bool ULoadoutBuilderComponent::CanEquipProjectile(const FName & ProjectileName) const
{
	if (ProjectileName == NAME_None) return true;

	GET_GAME_INSTANCE()

	// Projectile can only be equipped when it shares the same cartridge
	const AFirearm * ItemCDO = Cast<AFirearm>(GameInstance->GetItem(Loadout.ItemSlots[CurrentItemSlot].ItemClassName).GetDefaultObject());
	if (ItemCDO && ItemCDO->ProjectileClass && ItemCDO->ProjectileClass->GetFName() != ProjectileName)
	{
		ABaseProjectile * ProjectileCDO = GameInstance->GetProjectile(ProjectileName).GetDefaultObject();
		if (ProjectileCDO)
		{
			return ItemCDO->ProjectileClass.GetDefaultObject()->CompatibilityTag == ProjectileCDO->CompatibilityTag;
		}
	}

	return false;
}

void ULoadoutBuilderComponent::EquipProjectile(const FName & ProjectileName)
{
	if (CanEquipProjectile(ProjectileName))
	{
		GetCurrentSlot().ProjectileClassName = ProjectileName;
	}
}

bool ULoadoutBuilderComponent::CanEquipSight(const FName & SightName) const
{
	GET_GAME_INSTANCE()

	const USight * SightCDO = GameInstance->GetSight(SightName).GetDefaultObject();
	if (SightCDO)
	{
		// Sight can only be equipped when it shares at least one compatibility tag with the SightComponents ComponentTags
		const AItem * ItemCDO = GameInstance->GetItem(Loadout.ItemSlots[CurrentItemSlot].ItemClassName).GetDefaultObject();
		if (ItemCDO && ItemCDO->SightComponent && ItemCDO->SightComponent->GetVisibleFlag())
		{
			for (const FName & SightTag : ItemCDO->SightComponent->ComponentTags)
			{
				return SightCDO->CompatibilityTags.Contains(SightTag);
			}
		}

		return false;
	}

	return true;
}

void ULoadoutBuilderComponent::EquipSight(const FName & SightName)
{
	if (CanEquipSight(SightName))
	{
		GetCurrentSlot().SightClassName = SightName;
	}
}

bool ULoadoutBuilderComponent::CanEquipSuppressor() const
{
	GET_GAME_INSTANCE()

	const AFirearm * ItemCDO = Cast<AFirearm>(GameInstance->GetItem(Loadout.ItemSlots[CurrentItemSlot].ItemClassName).GetDefaultObject());
	if (ItemCDO)
	{
		return ItemCDO->SuppressorComponent->GetVisibleFlag();
	}

	return false;
}

void ULoadoutBuilderComponent::EquipSuppressor(bool bEquip)
{
	if (CanEquipSuppressor())
	{
		GetCurrentSlot().bHasSuppressor = bEquip;
	}
}

bool ULoadoutBuilderComponent::CanEquipAttachment(const FName & AttachmentName) const
{
	return true;
}

void ULoadoutBuilderComponent::EquipAttachment(const FName & AttachmentName)
{
	if (CanEquipAttachment(AttachmentName))
	{
		GetCurrentSlot().AttachmentComponentNames.Add(AttachmentName);
	}
}

bool ULoadoutBuilderComponent::CanEquipPerk(EPerkEnum Perk) const
{
	return true;
}

void ULoadoutBuilderComponent::EquipPerk(EPerkEnum Perk)
{
	if (CanEquipPerk(Perk))
	{
		Loadout.Perk = Perk;
	}
}