// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GhostCharacter.generated.h"


class AGuardCharacter;

UCLASS()
class GAMEJAMPROJECT_API AGhostCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGhostCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void GhostSucked(AGuardCharacter* GuardAI);
	void GhostReleased(AGuardCharacter* GuardAI);
	void StartTugOfWar(AGuardCharacter* GuardAI);

	bool GetIsBeingCaptured() const;
	bool GetHasWon() const;

private:
	float TugOfWarStrength{};  // Player's resistance level
	float CaptureProgress{};   // AI's capture progress
	bool bIsBeingCaptured{};   // Whether tug-of-war is active
	bool bHasWon{};

	FTimerHandle CaptureTimerHandle;  // Timer for continuous AI pulling

	void IncreaseResistance();  // Called when player presses the button
	void UpdateCaptureProgress(AGuardCharacter* GuardAI); // Called every second during capture
};
