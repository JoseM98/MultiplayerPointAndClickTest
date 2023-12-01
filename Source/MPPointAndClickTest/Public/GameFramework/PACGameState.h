// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PACGameState.generated.h"


UCLASS()
class MPPOINTANDCLICKTEST_API APACGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	APACGameState();

	void IncreaseAIScore();
	
protected:
	// AI Score
	UPROPERTY(ReplicatedUsing=OnRep_AIScore, BlueprintReadOnly, Category="PACGameState")
	int AIScore = 0;

	UFUNCTION()
	void OnRep_AIScore();
};
