#include "FPSTemplate.h"
#include "DamageType_Knife.h"

UDamageType_Knife::UDamageType_Knife()
	: Super()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> NewIcon(TEXT("Texture2D'/Game/FPSTemplate/HUD/Textures/DamageTypeIcons/T_Knife_Icon.T_Knife_Icon'"));
	Icon = NewIcon.Object;
}