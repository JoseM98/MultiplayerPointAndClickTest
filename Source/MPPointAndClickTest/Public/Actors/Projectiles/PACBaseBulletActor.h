// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PACBaseBulletActor.generated.h"

class UStaticMeshComp;
class UPrimitiveComponent;
class UProjectileMovementComponent;
class APawn;

UCLASS(Blueprintable)
class MPPOINTANDCLICKTEST_API APACBaseBulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APACBaseBulletActor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Bullet|Data")
	float Damage = 100.f;

	void SetOwnerPawn(APawn* OwnerPawnToSet);
	
	APawn* GetOwnerPawn() const ;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category="Bullet|Structure")
	UStaticMeshComponent* BulletMeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category="Bullet|Structure")
	UProjectileMovementComponent* ProjectileMovementComp;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Player who fires the bullet. Only valid on server side.
	UPROPERTY()
	APawn* OwnerPawn = nullptr;
};
