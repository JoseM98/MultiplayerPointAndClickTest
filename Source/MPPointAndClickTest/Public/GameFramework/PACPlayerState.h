// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PACPlayerState.generated.h"


UCLASS()
class MPPOINTANDCLICKTEST_API APACPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	APACPlayerState();

	void IncreasePlayerScore();

	
protected:
	virtual void BeginDestroy() override;

	// Player Score
	UPROPERTY(ReplicatedUsing=OnRep_PlayerScore, BlueprintReadOnly, Category="PACPlayerState")
	int PlayerScore = 0;

	UFUNCTION()
	void OnRep_PlayerScore();
};
