// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PACBasePlayerCharacter.h"
#include "PACShooterCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class APACBaseBulletActor;


UCLASS(Blueprintable)
class MPPOINTANDCLICKTEST_API APACShooterCharacter : public APACBasePlayerCharacter
{
	GENERATED_BODY()
	
public:
	APACShooterCharacter();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

	// Shooter Mapping Context. Particular actions that the tank will have.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings|Input")
	UInputMappingContext* ShooterClassMappingContext;
	
	// Attack Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings|Input")
	UInputAction* AttackAction;
	
	// CameraZoom Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings|Input")
	UInputAction* CameraZoomAction;

	// Type of bullet that player will fire.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings|Bullet")
	TSubclassOf<APACBaseBulletActor> BulletActorClass;

	// Place where the bullet is going to be spawned. It has to be outside colliders to avoid collision or overlap events on spawn.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FireLocation")
	USceneComponent* FireLocationComp;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings|Camera")
	float MinCameraZoom = 800.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings|Camera")
	float MaxCameraZoom = 3500.f;

	// Camera zoom speed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings|Camera")
	float ZoomSpeed = 200.f;

	// Value used when moving the camera for a more smooth result and avoid small steps.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings|Camera")
	float SmoothSpeed = 10.f;

	/* Time between shoots. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings|Attack")
	float FireCadence = 0.5;
	
	
protected:
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;
	
	UFUNCTION()
	void OnAttackAction();

	UFUNCTION()
	void OnCameraZoomAction(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestAttack();

	
private:
	float TargetCameraArmLocation;

	/* Controls shoot cadence (time between shoots) */
	bool bCanFireAgain = true;
	
	UPROPERTY()
	FTimerHandle CadenceTimerHandle;

	/* Calls every certain milliseconds while the player is shooting */
	UFUNCTION()
	void FireCadenceFinished();
};
