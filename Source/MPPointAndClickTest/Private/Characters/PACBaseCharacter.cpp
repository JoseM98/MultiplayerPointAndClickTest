// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PACBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/PACHealthComponent.h"
#include "GameFramework/PACGameState.h"
#include "GameFramework/PACPlayerState.h"


APACBaseCharacter::APACBaseCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	HealthComp = CreateDefaultSubobject<UPACHealthComponent>(TEXT("HealthComp"));
}

TSubclassOf<ACharacter> APACBaseCharacter::GetRespawnCharacterClass_Implementation() const
{
	return StaticClass();
}

void APACBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority() && HealthComp)
	{
		HealthComp->OnActorDeadDelegate.BindDynamic(this, &APACBaseCharacter::OnActorDead);
	}
}

void APACBaseCharacter::OnActorDead(const AActor* KillerActor)
{
	const APawn* KillerPawn = Cast<APawn>(KillerActor);
	if(!KillerPawn)
	{
		return;
	}
	
	// Increase player score.
	if(APACPlayerState* PACPlayerState = Cast<APACPlayerState>(KillerPawn->GetPlayerState()))
	{
		if(!PACPlayerState->IsABot())
		{
			PACPlayerState->IncreasePlayerScore();
			return;
		}
	}
	
	// If killerActor is not a player add points to AI in GameState. Also avoids that AI adds points when killing another AIs.
	const UWorld* World = GetWorld();
	if(World && !IsBotControlled())
	{
		if(APACGameState* GameState = Cast<APACGameState>(World->GetGameState()))
		{
			GameState->IncreaseAIScore();
		}
	}
}
