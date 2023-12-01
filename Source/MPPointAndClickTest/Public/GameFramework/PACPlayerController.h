// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PACPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class ACharacter;


UCLASS()
class MPPOINTANDCLICKTEST_API APACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APACPlayerController();
	
	// Destroy old character and call GameMode for respawning.
	UFUNCTION(BlueprintCallable, Category="PACPlayerController")
	void RespawnPawn(ACharacter* PawnToDelete, TSubclassOf<ACharacter> CharacterClass);

	// Default Mapping Context. Basic actions will be bound to the PC as we want them to be shared in case a player can use several types of vehicles. In this case the movement will be the same for all.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings|Input")
	UInputMappingContext* DefaultMappingContext;
	
	// Click Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings|Input")
	UInputAction* MovementAction;

	// Show scoreboard
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings|Input")
	UInputAction* ShowScoreboardAction;

protected:
	virtual void SetupInputComponent() override;
	
	virtual void BeginPlay() override;

	virtual void OnUnPossess() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnMoveAction();

	UFUNCTION()
	void OnShowScoreboard();
	
	UFUNCTION()
	void OnHideScoreboard();


private:
	TArray<FVector> PathTargetPoints;

	void MovePawn();
};
