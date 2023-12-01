// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PACBaseCharacter.generated.h"

class UPACHealthComponent;


UCLASS(Blueprintable)
class MPPOINTANDCLICKTEST_API APACBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APACBaseCharacter();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	UPACHealthComponent* HealthComp;

	UFUNCTION(BlueprintNativeEvent, Category="PACBaseCharacter|Respawn")
	TSubclassOf<ACharacter> GetRespawnCharacterClass() const;
	virtual TSubclassOf<ACharacter> GetRespawnCharacterClass_Implementation() const;

	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnActorDead(const AActor* KillerActor);
};
