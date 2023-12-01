// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PACGameModeBase.generated.h"

class ACharacter;


UCLASS()
class MPPOINTANDCLICKTEST_API APACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	APACGameModeBase();

	// Spawn a new character and assign its PlayerController.
	UFUNCTION(BlueprintCallable, Category="PACGameModeBase")
	void RespawnPlayerCharacter(APlayerController* PlayerController, TSubclassOf<ACharacter> CharacterClass);

	// Spawn a new bot and its default controller
	UFUNCTION(BlueprintCallable, Category="PACGameModeBase")
	void SpawnBotCharacter(TSubclassOf<ACharacter> CharacterClass);
	

protected:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	/* Current level PlayerStarts. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="PACGameModeBase")
	TArray<AActor*> PlayerStarts;
	
	// Spawn character at the location of one player start in the level, selected randomly.
	ACharacter* SpawnCharacterAtRandomPlayerStart(TSubclassOf<ACharacter> CharacterClass);
};
