#include "FPSTemplate.h"
#include "ConquestFlag.h"
#include "Soldier/Soldier.h"
#include "UnrealNetwork.h"

AConquestFlag::AConquestFlag(const FObjectInitializer & ObjectInitializer)
{
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = Mesh;
	FlagMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("FlagMesh"));
	FlagMesh->SetupAttachment(Mesh);
	FlagMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	SphereComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(Mesh);
	SphereComponent->SetSphereRadius(500.f, false);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CaptureSpeed = 0.1f;

	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.TickInterval = 0.1f;
}

void AConquestFlag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AConquestFlag, CapturedByTeam);
}

void AConquestFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int32 TeamASoldiers = 0;
	int32 TeamBSoldiers = 0;

	TArray<AActor *> OverlappingActors;
	SphereComponent->GetOverlappingActors(OverlappingActors);
	for (AActor * Actor : OverlappingActors)
	{
		if (Actor->IsA<ASoldier>())
		{
			ASoldier * Soldier = Cast<ASoldier>(Actor);
			if (!Soldier->IsDead())
			{
				const ETeamEnum Team = Soldier->GetTeam();

				if (Team == ETeamEnum::TeamA)
					++TeamASoldiers;
				else
					++TeamBSoldiers;
			}
		}
	}

	ETeamEnum CapturingTeam = ETeamEnum::None;
	if (TeamASoldiers > TeamBSoldiers)
		CapturingTeam = ETeamEnum::TeamA;
	else if (TeamBSoldiers > TeamASoldiers)
		CapturingTeam = ETeamEnum::TeamB;

	if (CapturingTeam != ETeamEnum::None)
	{
		if (CapturingTeam == CapturedByTeam)
		{
			if (HeightRatio < 1.f)
			{
				HeightRatio = FMath::Min(HeightRatio + CaptureSpeed * DeltaTime, 1.f);
				UpdateFlag();
			}
		}
		else
		{
			HeightRatio -= CaptureSpeed * DeltaTime;
			if (HeightRatio <= 0.f)
			{
				// Not set to 0 so we don't loose the time below 0
				HeightRatio = -HeightRatio;

				if (GetNetMode() != NM_Client)
					CapturedByTeam = CapturingTeam;
			}

			UpdateFlag();
		}
	}
}

void AConquestFlag::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MID = FlagMesh->CreateAndSetMaterialInstanceDynamic(0);
	UpdateFlag();
}

void AConquestFlag::OnRep_CapturedByTeam(ETeamEnum OldTeam)
{
	if (OldTeam != CapturedByTeam)
	{
		HeightRatio = 0.f;
		UpdateFlag();
	}
}

void AConquestFlag::UpdateFlag()
{
	// Update location
	FVector NewLocation = FlagMesh->GetRelativeLocation();
	NewLocation.Z = FlagHeightRange.X + HeightRatio * (FlagHeightRange.Y - FlagHeightRange.X);
	FlagMesh->SetRelativeLocation(NewLocation);

	// Update material
	float Value;
	switch (CapturedByTeam)
	{
	case (ETeamEnum::None) :
		Value = 0.f;
		break;
	case (ETeamEnum::TeamA) :
		Value = HeightRatio;
		break;
	case (ETeamEnum::TeamB) :
		Value = -HeightRatio;
		break;
	default:
		Value = 0.f;
	}

	MID->SetScalarParameterValue(FName(TEXT("CapturingTeam")), Value);
}