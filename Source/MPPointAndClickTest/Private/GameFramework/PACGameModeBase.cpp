// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameFramework/PACGameModeBase.h"

#include "GameFramework/PACGameState.h"
#include "GameFramework/PACHUD.h"
#include "GameFramework/PACPlayerController.h"
#include "GameFramework/PACPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PACShooterCharacter.h"
#include "Utils/PACGameplayStatics.h"


APACGameModeBase::APACGameModeBase()
{
	PlayerControllerClass = APACPlayerController::StaticClass();	// Overriden in blueprint
	GameStateClass = APACGameState::StaticClass();
	PlayerStateClass = APACPlayerState::StaticClass();
	DefaultPawnClass = APACShooterCharacter::StaticClass();			// Overriden in blueprint
	HUDClass = APACHUD::StaticClass();								// Overriden in blueprint
}

void APACGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
}

void APACGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Need to update HUD on listen clients
	UPACGameplayStatics::UpdateHUD(this);
}


void APACGameModeBase::RespawnPlayerCharacter(APlayerController* PlayerController, TSubclassOf<ACharacter> CharacterClass)
{
	APACPlayerController* PACPlayerController = Cast<APACPlayerController>(PlayerController);
	if(!IsValid(PACPlayerController) || !IsValid(CharacterClass))
	{
		return;
	}

	if(PlayerStarts.Num() <= 0)
	{
		return;
	}
	
	ACharacter* Character = SpawnCharacterAtRandomPlayerStart(CharacterClass);

	// Wait for character spawn and possess it.
	FTimerHandle PossessTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(PossessTimerHandle, [this, PACPlayerController, Character]() {
		if(Character)
		{
			PACPlayerController->Possess(Character);
		}
	}, 0.2f, false);
}

void APACGameModeBase::SpawnBotCharacter(TSubclassOf<ACharacter> CharacterClass)
{
	ACharacter* Character = SpawnCharacterAtRandomPlayerStart(CharacterClass);
	if(Character)
	{
		Character->SpawnDefaultController();
	}
}

ACharacter* APACGameModeBase::SpawnCharacterAtRandomPlayerStart(TSubclassOf<ACharacter> CharacterClass)
{
	if(!CharacterClass)
	{
		return nullptr;
	}
	
	// Get a random player start and spawn character on it
	const int32 Number = FMath::RandRange(0, PlayerStarts.Num() - 1);
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	return GetWorld()->SpawnActor<ACharacter>(CharacterClass, PlayerStarts[Number]->GetActorLocation(), PlayerStarts[Number]->GetActorRotation(), ActorSpawnParams);
}
