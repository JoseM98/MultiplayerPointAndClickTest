// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PACShooterCharacter.h"
#include "GameFramework/PACPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/Projectiles/PACBaseBulletActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utils/PACGameplayStatics.h"


DEFINE_LOG_CATEGORY_STATIC(LogPACShooterCharacter, Log, All);

APACShooterCharacter::APACShooterCharacter()
{
	// Default bullet.
	BulletActorClass = APACBaseBulletActor::StaticClass();

	FireLocationComp = CreateDefaultSubobject<USceneComponent>(TEXT("FireLocationComp"));
	FireLocationComp->SetupAttachment(RootComponent);
	FireLocationComp->SetRelativeLocation(FVector(120.f, 0.f, 50.f));
}

void APACShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	TargetCameraArmLocation = SpringArmComp->TargetArmLength;
}

void APACShooterCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UPACGameplayStatics::UpdateHUD(this);
}

void APACShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APACPlayerController* PlayerController = Cast<APACPlayerController>(GetController());
	if(!PlayerController)
	{
		UE_LOG(LogPACShooterCharacter, Warning, TEXT("APACShooterCharacter::BindInput: PlayerController not valid."));
		return;
	}
	
	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(ShooterClassMappingContext, 0);
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APACShooterCharacter::OnAttackAction);

		EnhancedInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Started, this, &APACShooterCharacter::OnCameraZoomAction);
		EnhancedInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Ongoing, this, &APACShooterCharacter::OnCameraZoomAction);
	}
}

void APACShooterCharacter::OnAttackAction()
{
	// Fire cooldown
	if(bCanFireAgain)
	{
		bCanFireAgain = false;
		GetWorld()->GetTimerManager().SetTimer(CadenceTimerHandle, this, &APACShooterCharacter::FireCadenceFinished, FireCadence, false);
		Server_RequestAttack();
	}
}

void APACShooterCharacter::FireCadenceFinished()
{
	bCanFireAgain = true;
}

void APACShooterCharacter::OnCameraZoomAction(const FInputActionValue& Value)
{
	const float FloatValue = Value.Get<float>();
	if(FloatValue == 0.f)
	{
		return;
	}
	
	TargetCameraArmLocation += FloatValue * ZoomSpeed;
	TargetCameraArmLocation = UKismetMathLibrary::FClamp(TargetCameraArmLocation, MinCameraZoom, MaxCameraZoom);
}

void APACShooterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Calculate final camera arm length with a smooth result.
	const float FinalLocation = UKismetMathLibrary::FInterpTo(SpringArmComp->TargetArmLength, TargetCameraArmLocation, DeltaSeconds, SmoothSpeed);
	SpringArmComp->TargetArmLength = FinalLocation;
}

void APACShooterCharacter::Server_RequestAttack_Implementation()
{
	if(!HasAuthority())
	{
		return;
	}

	if(!BulletActorClass)
	{
		UE_LOG(LogPACShooterCharacter, Warning, TEXT("APACShooterCharacter::Server_RequestAttack_Implementation: BulletClass not valid."));
		return;
	}

	// Spawn bullet
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APACBaseBulletActor* BaseBulletActor = GetWorld()->SpawnActor<APACBaseBulletActor>(BulletActorClass, FireLocationComp->GetComponentLocation(), GetActorRotation(), ActorSpawnParams);
	if(BaseBulletActor)
	{
		BaseBulletActor->SetOwnerPawn(this);
	}
}

bool APACShooterCharacter::Server_RequestAttack_Validate()
{
	// ToDo: Check the time between shots in this method to avoid cheating.
	return true;
}