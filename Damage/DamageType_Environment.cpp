#include "FPSTemplate.h"
#include "DamageType_Environment.h"

UDamageType_Environment::UDamageType_Environment()
	: Super()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> NewIcon(TEXT("Texture2D'/Game/FPSTemplate/HUD/Textures/DamageTypeIcons/T_Environment_Icon.T_Environment_Icon'"));
	Icon = NewIcon.Object;

	bCausedByWorld = true;
}