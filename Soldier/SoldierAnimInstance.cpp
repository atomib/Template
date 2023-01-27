#include "FPSTemplate.h"
#include "SoldierAnimInstance.h"
#include "Soldier/SoldierMovementComponent.h"

void USoldierAnimInstance::NativeInitializeAnimation()
{
	Soldier = Cast<ASoldier>(GetOwningActor());
}

void USoldierAnimInstance::InitItem(AItem * Item)
{
	if (Item)
	{
		// Update item sepcific sequences
		ItemIdleSequence = Item->CharacterIdleSequence;
		ItemAimSequence = Item->CharacterAimSequence;
		ItemSprintSequence = Item->CharacterSprintSequence;
	}
	else
	{
		ItemIdleSequence = NULL;
		ItemAimSequence = NULL;
		ItemSprintSequence = NULL;
	}
}

void USoldierAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (Soldier)
	{
		USoldierMovementComponent * SoldierMovement = Cast<USoldierMovementComponent>(Soldier->GetCharacterMovement());

		// Update important properties for the anim graph
		bFirstPersonMode = Soldier->IsViewTarget();
		ViewPitch = FMath::ClampAngle(Soldier->GetViewPitch(), -90.f, 90.f);
		AimRatio = Soldier->AimRatio;
		Soldier->GetArmsOffset(ArmsOffset, ArmsRotation);

		LeanRatio = FMath::FInterpTo(LeanRatio, Soldier->LeanRatio, DeltaTime, 8.f);
		
		const FVector Velocity = Soldier->GetCharacterMovement()->Velocity;
		const float Speed = Velocity.Size();

		const float NewSpeedRatio = FMath::Clamp(Speed / (Soldier->bIsCrouched ? SoldierMovement->MaxWalkSpeedCrouched : SoldierMovement->MaxSprintSpeed), 0.f, 1.f);
		SpeedRatio = FMath::FInterpTo(SpeedRatio, NewSpeedRatio, DeltaTime, FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1.f), FVector2D(128.f, 8.f), SpeedRatio));

		const float NewUpperBodySpeed = (Speed < SoldierMovement->MaxWalkSpeed) ? FMath::GetMappedRangeValueClamped(FVector2D(0.f, SoldierMovement->MaxWalkSpeed), FVector2D(0.f, 0.25f), Speed) : FMath::GetMappedRangeValueClamped(FVector2D(SoldierMovement->MaxWalkSpeed, SoldierMovement->MaxSprintSpeed), FVector2D(0.25f, 1.f), Speed);
		UpperBodySpeedRatio = FMath::FInterpTo(UpperBodySpeedRatio, Soldier->GetItemTask() != EItemTaskEnum::Fire ? NewUpperBodySpeed : 0.f, DeltaTime, 8.f);

		float NewDirection = FMath::ClampAngle(FRotationMatrix::MakeFromX(Velocity).Rotator().Yaw - Soldier->GetActorRotation().Yaw, -179.999f, 179.999f);
		if (FMath::Abs(Speed) < 10.f)
		{
			NewDirection = 0.f;
		}
		Direction = NewDirection;//FMath::FInterpTo(Direction, NewDirection, DeltaTime, 8.f);

		MovementMode = Soldier->GetCharacterMovement()->MovementMode;
		bCrouching = Soldier->bIsCrouched;

		// Lower body offset
		const float MaxLowerBodyOffset = 30.f;
		const float NewYawRotation = Soldier->GetActorRotation().Yaw;
		const float RotationSpeed = CachedSoldierYaw - NewYawRotation;
		CachedSoldierYaw = NewYawRotation;
		LowerBodyOffset = FMath::ClampAngle(LowerBodyOffset + RotationSpeed, -MaxLowerBodyOffset, MaxLowerBodyOffset);

		if (SpeedRatio > 0.f)
			LowerBodyOffset = FMath::FInterpTo(LowerBodyOffset, 0.f, DeltaTime, SpeedRatio * 16.f);
	}
}