#include "FPSTemplate.h"
#include "BaseProjectile.h"


ABaseProjectile::ABaseProjectile(const FObjectInitializer & ObjectInitializer)
{
	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UBaseProjectileMovementComponent>(this, TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = SphereCollision;
	ProjectileMovement->bRotationFollowsVelocity = true;

	SphereCollision = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	SphereCollision->SetCollisionProfileName(FName("Projectile"));
	SphereCollision->bReturnMaterialOnMove = true;
	SphereCollision->CanCharacterStepUpOn = ECB_No;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetCollisionProfileName(FName("NoCollision"));

	InitialLifeSpan = 2.f;
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ProjectileMovement->OnProjectileBounce.AddDynamic(this, &ABaseProjectile::OnProjectileHit);
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &ABaseProjectile::OnProjectileStop);

	SphereCollision->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FlybySound && GetWorld()->ViewLocationsRenderedLastFrame.Num())
	{
		const FVector ViewLocation = GetWorld()->ViewLocationsRenderedLastFrame[0];
		const FVector Location = RootComponent->GetComponentLocation();
		const float NewDistSquared = (Location - ViewLocation).SizeSquared();

		if (NewDistSquared > ListenerDistSquared)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, FlybySound, Location);
			FlybySound = NULL;
		}

		ListenerDistSquared = NewDistSquared;
	}
}

void ABaseProjectile::OnProjectileStop(const FHitResult& HitResult)
{
	Destroy();
}

void ABaseProjectile::OnProjectileHit(const FHitResult & HitResult, const FVector & ImpactVelocity)
{
	
}

void ABaseProjectile::SpawnImpactEffect(const FHitResult & HitResult)
{

}