// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PACHUD.h"
#include "UI/PACScoreboardUserWidget.h"

APACHUD::APACHUD()
{
}

void APACHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if(ScoreboardWidgetClass && !ScoreboardUserWidget)
	{
		ScoreboardUserWidget = CreateWidget<UPACScoreboardUserWidget>(PlayerOwner, ScoreboardWidgetClass);
		if(ScoreboardUserWidget)
		{
			ScoreboardUserWidget->AddToViewport();
			ToggleScoreboardVisibility(false);
			UpdateScoreboard();
		}
	}
}

void APACHUD::ToggleScoreboardVisibility(bool bShow)
{
	if(ScoreboardUserWidget)
	{
		ScoreboardUserWidget->SetVisibility(bShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void APACHUD::UpdateScoreboard()
{
	if(ScoreboardUserWidget)
	{
		ScoreboardUserWidget->UpdateScoreboard();
	}
}
