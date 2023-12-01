// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PACBotBaseCharacter.h"
#include "PACInfantryBotCharacter.generated.h"

class APACBaseBulletActor;

/**
 * 
 */
UCLASS()
class MPPOINTANDCLICKTEST_API APACInfantryBotCharacter : public APACBotBaseCharacter
{
	GENERATED_BODY()

public:
	APACInfantryBotCharacter();

	// Place where the bullet is going to be spawned. It has to be outside colliders to avoid collision or overlap events on spawn.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FireLocation")
	USceneComponent* FireLocationComp;

	UFUNCTION(BlueprintCallable, Category="InfantryBot")
	void Fire();

	// Bullet that this bot will fire
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings|BulletClass")
	TSubclassOf<APACBaseBulletActor> BulletActorClass;
};
