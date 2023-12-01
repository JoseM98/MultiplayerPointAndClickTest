// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PACHealthComponent.generated.h"

class UDamageType;
class AController;
class UPACHealthComponent;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnActorDeadDelegate, const AActor*, KillerActor);


UCLASS( ClassGroup=(COOP), meta=(BlueprintSpawnableComponent) )
class MPPOINTANDCLICKTEST_API UPACHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPACHealthComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HealthComponent")
	bool bDestroyOnDeath = false;

	float GetHealth() const { return Health; }

	// Only fires on server
	FOnActorDeadDelegate OnActorDeadDelegate;

	
protected:
	virtual void BeginPlay() override;

	bool bIsDead = false;

	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="HealthComponent")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="HealthComponent")
	float MaxHealth = 100.f;

	UFUNCTION()
	void Handler_TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
