// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PACPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/PACGameModeBase.h"
#include "GameFramework/PACHUD.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/Character.h"


APACPlayerController::APACPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void APACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Lock mouse into the game
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, nullptr, EMouseLockMode::LockAlways, false);

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void APACPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	// Remove path points.
	PathTargetPoints.Empty();
}

void APACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Started, this, &APACPlayerController::OnMoveAction);

		EnhancedInputComponent->BindAction(ShowScoreboardAction, ETriggerEvent::Started, this, &APACPlayerController::OnShowScoreboard);
		EnhancedInputComponent->BindAction(ShowScoreboardAction, ETriggerEvent::Canceled, this, &APACPlayerController::OnHideScoreboard);
		EnhancedInputComponent->BindAction(ShowScoreboardAction, ETriggerEvent::Completed, this, &APACPlayerController::OnHideScoreboard);
	}
}

void APACPlayerController::OnMoveAction()
{
	const APawn* ControlledPawn = GetPawn();
	if(!ControlledPawn)
	{
		return;
	}
	
	// ToDo: This can be improved by using a custom collision channel, so that it is used only by objects that we want to be able to detect a click, such as the floor.
	FHitResult Hit;
	const bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	if(bHitSuccessful)
	{
		PathTargetPoints.Empty();

		// Get nav points needed to reach the destination using unreal pathfinding.
		const UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), ControlledPawn->GetActorLocation(), Hit.Location, this);
		if(NavigationPath)
		{
			PathTargetPoints = NavigationPath->PathPoints;
		}
	}
}

void APACPlayerController::OnShowScoreboard()
{
	if(APACHUD* PACHUD = Cast<APACHUD>(GetHUD()))
	{
		PACHUD->ToggleScoreboardVisibility(true);
	}
}

void APACPlayerController::OnHideScoreboard()
{
	if(APACHUD* PACHUD = Cast<APACHUD>(GetHUD()))
	{
		PACHUD->ToggleScoreboardVisibility(false);
	}
}

void APACPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MovePawn();
}

void APACPlayerController::MovePawn()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn || PathTargetPoints.IsEmpty())
	{
		return;
	}
	
	float Distance = (PathTargetPoints[0] - ControlledPawn->GetActorLocation()).Size2D();
	if(Distance < 0.f)
	{
		Distance *= -1.f;
	}
	
	if(Distance < 50.f) // Acceptable radius to reach this point. ToDo: This comparison can be parametrized and improved based on pawn size properties.
	{
		PathTargetPoints.RemoveAt(0);
	}

	if(!PathTargetPoints.IsEmpty())
	{
		const FVector WorldDirection = (PathTargetPoints[0] - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void APACPlayerController::RespawnPawn(ACharacter* PawnToDelete, TSubclassOf<ACharacter> CharacterClass)
{
	const UWorld* World = GetWorld();
	if(!World)
	{
		return;
	}
	
	APACGameModeBase* GameMode = Cast<APACGameModeBase>(World->GetAuthGameMode());
	if(IsValid(GameMode))
	{
		GameMode->RespawnPlayerCharacter(this, CharacterClass);
	}
	
	PawnToDelete->Destroy();
}
