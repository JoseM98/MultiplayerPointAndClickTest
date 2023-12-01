// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PACBasePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PACGameState.h"
#include "GameFramework/PACPlayerController.h"
#include "GameFramework/SpringArmComponent.h"


APACBasePlayerCharacter::APACBasePlayerCharacter()
{
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Spring Arm Component
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	SpringArmComp->TargetArmLength = 2200.f;
	SpringArmComp->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArmComp->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void APACBasePlayerCharacter::OnActorDead(const AActor* KillerActor)
{
	Super::OnActorDead(KillerActor);
	
	StartPlayerDieProcess();
}

void APACBasePlayerCharacter::StartPlayerDieProcess()
{
	Multicast_PlayerDead();
	RespawnPlayerController();
}

void APACBasePlayerCharacter::Multicast_PlayerDead_Implementation()
{
	// Here VFX can be added to be played on client.
	
	// Disable relevant elements before destroying the character
	GetCapsuleComponent()->SetEnableGravity(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
}

void APACBasePlayerCharacter::RespawnPlayerController()
{
	if(APACPlayerController* PACPlayerController = Cast<APACPlayerController>(GetController()))
	{
		PACPlayerController->UnPossess();
		PACPlayerController->RespawnPawn(this, GetRespawnCharacterClass());
	}
}
