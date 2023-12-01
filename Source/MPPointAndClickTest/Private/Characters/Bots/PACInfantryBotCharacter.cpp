// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Bots/PACInfantryBotCharacter.h"
#include "Actors/Projectiles/PACBaseBulletActor.h"


APACInfantryBotCharacter::APACInfantryBotCharacter()
{
	BulletActorClass = APACBaseBulletActor::StaticClass();

	FireLocationComp = CreateDefaultSubobject<USceneComponent>(TEXT("FireLocationComp"));
	FireLocationComp->SetupAttachment(RootComponent);
	FireLocationComp->SetRelativeLocation(FVector(120.f, 0.f, 50.f));
}

void APACInfantryBotCharacter::Fire()
{
	if(!HasAuthority())
	{
		return;
	}

	// Spawn bullet
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APACBaseBulletActor* BaseBulletActor = GetWorld()->SpawnActor<APACBaseBulletActor>(BulletActorClass, FireLocationComp->GetComponentLocation(), GetActorRotation(), ActorSpawnParams);
	if(BaseBulletActor)
	{
		BaseBulletActor->SetOwnerPawn(this);
	}
}
