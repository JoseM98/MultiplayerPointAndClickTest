// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PACGameplayStatics.generated.h"


UCLASS()
class MPPOINTANDCLICKTEST_API UPACGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI|Scoreboard", meta = (WorldContext = "WorldContextObject"), BlueprintCosmetic)
	static void UpdateHUD(const UObject* WorldContextObject);
};