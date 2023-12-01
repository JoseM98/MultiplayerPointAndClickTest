// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PACBaseCharacter.h"
#include "PACBasePlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS(Blueprintable)
class MPPOINTANDCLICKTEST_API APACBasePlayerCharacter : public APACBaseCharacter
{
	GENERATED_BODY()

public:
	APACBasePlayerCharacter();
	

protected:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	UCameraComponent* CameraComp;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	USpringArmComponent* SpringArmComp;

	void StartPlayerDieProcess();

	void RespawnPlayerController();

	// Animate player death in clients. Unreliable as is only used for visual feedback.
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayerDead();
	
	virtual void OnActorDead(const AActor* KillerActor) override;
};
