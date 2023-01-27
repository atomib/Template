#include "FPSTemplate.h"
#include "DamageType_Suicide.h"

UDamageType_Suicide::UDamageType_Suicide()
	: Super()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> NewIcon(TEXT("Texture2D'/Game/FPSTemplate/HUD/Textures/DamageTypeIcons/T_Suicide_Icon.T_Suicide_Icon'"));
	Icon = NewIcon.Object;
}