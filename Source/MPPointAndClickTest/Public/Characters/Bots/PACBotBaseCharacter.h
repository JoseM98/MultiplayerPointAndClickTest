// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PACBaseCharacter.h"
#include "PACBotBaseCharacter.generated.h"

UCLASS(Blueprintable)
class MPPOINTANDCLICKTEST_API APACBotBaseCharacter : public APACBaseCharacter
{
	GENERATED_BODY()

public:
	APACBotBaseCharacter();

	
protected:
	virtual void OnActorDead(const AActor* KillerActor) override;
};
