// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectiles/PACBaseBulletActor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PACHealthComponent.h"
#include "GameFramework/Pawn.h"


APACBaseBulletActor::APACBaseBulletActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	// Bullet mesh
	BulletMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMeshComp"));
	RootComponent = BulletMeshComp;
	BulletMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BulletMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BulletMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
	BulletMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	BulletMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// Projectile Movement comp
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	this->AddOwnedComponent(ProjectileMovementComp);
	ProjectileMovementComp->UpdatedComponent = BulletMeshComp;
	ProjectileMovementComp->InitialSpeed = 4000.0f;
	ProjectileMovementComp->MaxSpeed = 4000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = false;
	ProjectileMovementComp->ProjectileGravityScale = 0.1f;

	InitialLifeSpan = 4.0f;

	bReplicates = true;
	SetReplicateMovement(true);
}

void APACBaseBulletActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(!HasAuthority() || !OtherActor)
	{
		return;
	}

	// Prevent the firing pawn from damaging itself. If we want to allow this, this can be parameterised.
	if(OtherActor == OwnerPawn)
	{
		return;
	}

	// Only apply damage to actors that have PACHealthComponent
	if(OtherActor->FindComponentByClass<UPACHealthComponent>())
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), OwnerPawn ? Cast<AActor>(OwnerPawn) : this, nullptr);
	}

	Destroy();
}

void APACBaseBulletActor::SetOwnerPawn(APawn* OwnerPawnToSet)
{
	OwnerPawn = OwnerPawnToSet;
}

APawn* APACBaseBulletActor::GetOwnerPawn() const
{
	return OwnerPawn;
}


