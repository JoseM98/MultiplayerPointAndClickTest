// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PACScoreboardUserWidget.generated.h"


UCLASS()
class MPPOINTANDCLICKTEST_API UPACScoreboardUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateScoreboard();

protected:
	virtual void NativeOnInitialized() override;
};
