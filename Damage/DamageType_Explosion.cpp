#include "FPSTemplate.h"
#include "DamageType_Explosion.h"

UDamageType_Explosion::UDamageType_Explosion()
	: Super()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> NewIcon(TEXT("Texture2D'/Game/FPSTemplate/HUD/Textures/DamageTypeIcons/T_Explosion_Icon.T_Explosion_Icon'"));
	Icon = NewIcon.Object;

	KillReward = 200;
}