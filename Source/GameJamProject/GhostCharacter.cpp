// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GuardAi/GuardCharacter.h"

// Sets default values
AGhostCharacter::AGhostCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGhostCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGhostCharacter::IncreaseResistance()
{
	if (bIsBeingCaptured)
	{
		TugOfWarStrength += 15.0f; // Player resists
		UE_LOG(LogTemp, Warning, TEXT("Player is resisting! Strength: %f"), TugOfWarStrength);
	}
}

void AGhostCharacter::UpdateCaptureProgress(AGuardCharacter* GuardAI)
{
	if (!bIsBeingCaptured || !GuardAI) return;

	CaptureProgress += 10.0f;  // AI pulls the ghost closer

	if (TugOfWarStrength > 0)
	{
		CaptureProgress -= TugOfWarStrength; // Player resists
		TugOfWarStrength = FMath::Max(0.0f, TugOfWarStrength - 5.0f); // Decay resistance over time
	}

	// Debug message above the player
	FVector DebugLocation = GetActorLocation() + FVector(0, 0, 100);
	FString DebugText = FString::Printf(TEXT("Capture Progress: %.0f%%"), CaptureProgress);
	DrawDebugString(GetWorld(), DebugLocation, DebugText, nullptr, FColor::Red, 0.1f, true);

	// Check win/loss conditions
	if (CaptureProgress >= 100.0f)
	{
		bHasWon = false;
		GetWorldTimerManager().ClearTimer(CaptureTimerHandle);
		GhostSucked(GuardAI);
		bIsBeingCaptured = false;
	}
	else if (CaptureProgress <= 0.0f)
	{
		bHasWon = true;
		GuardAI->Stun();
		GetWorldTimerManager().ClearTimer(CaptureTimerHandle);
		UCharacterMovementComponent* TargetMovement{ FindComponentByClass<UCharacterMovementComponent>() };
		if (TargetMovement)
		{
			TargetMovement->SetMovementMode(MOVE_Flying);
		}
		bIsBeingCaptured = false;
	}
}

void AGhostCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Called every frame
void AGhostCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGhostCharacter::GhostSucked(AGuardCharacter* GuardAI)
{
	UCharacterMovementComponent* TargetMovement{ FindComponentByClass<UCharacterMovementComponent>() };
	if (TargetMovement)
	{
		TargetMovement->StopMovementImmediately();
		TargetMovement->SetMovementMode(MOVE_None);
	}

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	AttachToComponent(GuardAI->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("PlayerSocket"));
	
	SetActorHiddenInGame(true);
	GetMesh()->SetVisibility(false);
	
	FVector HoldPosition {GuardAI->GetActorLocation() + (GuardAI->GetActorForwardVector() * 100.0f) + FVector(0, 0, 50)};
	SetActorLocation(HoldPosition);
}

void AGhostCharacter::GhostReleased(AGuardCharacter* GuardAI)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	SetActorHiddenInGame(false);
	GetMesh()->SetVisibility(true);
	
    SetActorLocation(FVector{ 0,0,GuardAI->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()*2});
    
    UCharacterMovementComponent* TargetMovement{ FindComponentByClass<UCharacterMovementComponent>() };
    if (TargetMovement)
    {
    	TargetMovement->SetMovementMode(MOVE_Flying);
    }
}

void AGhostCharacter::StartTugOfWar(AGuardCharacter* GuardAI)
{
	if (bIsBeingCaptured) return;

	bHasWon = false;
	bIsBeingCaptured = true;
	TugOfWarStrength = 0.0f;
	CaptureProgress = 50.0f;
	
	UCharacterMovementComponent* TargetMovement{ FindComponentByClass<UCharacterMovementComponent>() };
	if (TargetMovement)
	{
		TargetMovement->StopMovementImmediately();
		TargetMovement->SetMovementMode(MOVE_None);
	}

	GetWorldTimerManager().SetTimer(CaptureTimerHandle, [this, GuardAI](){this->UpdateCaptureProgress(GuardAI);}, 0.5f, true);
}

bool AGhostCharacter::GetIsBeingCaptured() const
{
	return bIsBeingCaptured;
}

bool AGhostCharacter::GetHasWon() const
{
	return bHasWon;
}