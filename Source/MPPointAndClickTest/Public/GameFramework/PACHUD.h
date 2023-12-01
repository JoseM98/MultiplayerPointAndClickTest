// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PACHUD.generated.h"

class UPACScoreboardUserWidget;


UCLASS()
class MPPOINTANDCLICKTEST_API APACHUD : public AHUD
{
	GENERATED_BODY()

public:
	APACHUD();
	
	UPROPERTY()
	UPACScoreboardUserWidget* ScoreboardUserWidget;

	void ToggleScoreboardVisibility(bool bShow);

	void UpdateScoreboard();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings|ScoreboardWidget")
	TSubclassOf<UPACScoreboardUserWidget> ScoreboardWidgetClass;
};
