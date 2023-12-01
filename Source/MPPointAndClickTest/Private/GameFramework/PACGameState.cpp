// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PACGameState.h"
#include "Net/UnrealNetwork.h"
#include "Utils/PACGameplayStatics.h"


APACGameState::APACGameState()
{
}

void APACGameState::IncreaseAIScore()
{
	if(!HasAuthority())
	{
		return;
	}
	
	AIScore++;
	OnRep_AIScore(); // for listen clients.
}

void APACGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APACGameState, AIScore);
}

void APACGameState::OnRep_AIScore()
{
	UPACGameplayStatics::UpdateHUD(this);
}
