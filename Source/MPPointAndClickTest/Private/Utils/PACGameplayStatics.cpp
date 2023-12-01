// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/PACGameplayStatics.h"
#include "GameFramework/PACHUD.h"


void UPACGameplayStatics::UpdateHUD(const UObject* WorldContextObject)
{
	if(!WorldContextObject)
	{
		return;
	}

	if(const UWorld* World = WorldContextObject->GetWorld())
	{
		if(const APlayerController* PC = World->GetFirstPlayerController())
		{
			if(APACHUD* PACHUD = Cast<APACHUD>(PC->GetHUD()))
			{
				PACHUD->UpdateScoreboard();
			}
		}
	}
}
