#include "FPSTemplate.h"
#include "LoadoutSave.h"

ULoadoutSave::ULoadoutSave()
{
	Loadouts.SetNum(6);
}

void ULoadoutSave::SetLoadout(uint8 Slot, const FLoadoutStruct & NewLoadout)
{
	Loadouts[Slot] = NewLoadout;
}

FLoadoutStruct & ULoadoutSave::GetCurrentLoadout()
{
	return Loadouts[CurrentSlot];
}