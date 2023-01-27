#include "FPSTemplate.h"
#include "DamageType_Headshot.h"

UDamageType_Headshot::UDamageType_Headshot()
	: Super()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> NewIcon(TEXT("Texture2D'/Game/FPSTemplate/HUD/Textures/DamageTypeIcons/T_Headshot_Icon.T_Headshot_Icon'"));
	Icon = NewIcon.Object;

	KillReward = 600;
}