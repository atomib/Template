#include "FPSTemplate.h"
#include "DamageType_Bullet.h"

UDamageType_Bullet::UDamageType_Bullet()
	: Super()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> NewIcon(TEXT("Texture2D'/Game/FPSTemplate/HUD/Textures/DamageTypeIcons/T_Bullet_Icon.T_Bullet_Icon'"));
	Icon = NewIcon.Object;

	KillReward = 500;
}