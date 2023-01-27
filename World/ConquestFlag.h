#pragma once

#include "GameFramework/Actor.h"
#include "ConquestFlag.generated.h"

UCLASS()
class FPSTEMPLATE_API AConquestFlag : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
		UMaterialInstanceDynamic * MID;

	UPROPERTY(ReplicatedUsing = OnRep_CapturedByTeam)
		ETeamEnum CapturedByTeam;

	// Capture percentage
	float HeightRatio;

public:	
	UPROPERTY(EditAnywhere, Category = Components)
		UStaticMeshComponent * Mesh;

	UPROPERTY(EditAnywhere, Category = Components)
		UStaticMeshComponent * FlagMesh;

	UPROPERTY(EditAnywhere, Category = Components)
		USphereComponent * SphereComponent;

	// Z offset to mesh between which the flag will be slided up/down
	UPROPERTY(EditAnywhere, Category = Components)
		FVector2D FlagHeightRange;

	// Rate at which the flag will be captured, % per s
	UPROPERTY(EditAnywhere, Category = Components)
		float CaptureSpeed;

	AConquestFlag(const FObjectInitializer & ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	// Return the team that currently controls this flag
	ETeamEnum GetControllingTeam() const { return HeightRatio == 1.f ? CapturedByTeam : ETeamEnum::None; }

private:
	UFUNCTION()
		void OnRep_CapturedByTeam(ETeamEnum OldTeam);

	void UpdateFlag();
	
};
