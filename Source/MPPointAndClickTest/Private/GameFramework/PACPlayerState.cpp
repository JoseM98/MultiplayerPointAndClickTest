// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PACPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Utils/PACGameplayStatics.h"


APACPlayerState::APACPlayerState()
{
}

void APACPlayerState::IncreasePlayerScore()
{
	if(!HasAuthority())
	{
		return;
	}
	
	PlayerScore++;
	OnRep_PlayerScore();
}

void APACPlayerState::BeginDestroy()
{
	Super::BeginDestroy();

	UPACGameplayStatics::UpdateHUD(this);
}

void APACPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APACPlayerState, PlayerScore);
}

void APACPlayerState::OnRep_PlayerScore()
{
	UPACGameplayStatics::UpdateHUD(this);
}
