// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PACHealthComponent.h"
#include "Net/UnrealNetwork.h"


UPACHealthComponent::UPACHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UPACHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
	// Only hook if we are server
	if(GetOwnerRole() == ROLE_Authority)
	{
		if(AActor* MyOwner = GetOwner())
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UPACHealthComponent::Handler_TakeAnyDamage);
		}
	}
}

void UPACHealthComponent::OnRep_Health()
{
	bIsDead = Health <= 0.0f;
	
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Health Changed: %s, Pawn: %s"), *FString::SanitizeFloat(Health), GetOwner() ? *GetOwner()->GetName() : TEXT("")));
	}
}

void UPACHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPACHealthComponent, Health);
}

void UPACHealthComponent::Handler_TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	if(Damage <= 0.0f || bIsDead) return;

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	OnRep_Health(); // modifies bIsDead

	if(bIsDead)
	{
		OnActorDeadDelegate.ExecuteIfBound(DamageCauser);
		
		if(bDestroyOnDeath)
		{
			GetOwner()->Destroy();
		}
	}
}


