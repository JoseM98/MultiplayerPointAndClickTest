// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Bots/PACBotBaseCharacter.h"
#include "Components/PACHealthComponent.h"
#include "GameFramework/PACGameModeBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APACBotBaseCharacter::APACBotBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComp->bDestroyOnDeath = true;
}

void APACBotBaseCharacter::OnActorDead(const AActor* KillerActor)
{
	Super::OnActorDead(KillerActor);

	if(APACGameModeBase* PACGameMode = Cast<APACGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		PACGameMode->SpawnBotCharacter(GetRespawnCharacterClass());
	}

}
