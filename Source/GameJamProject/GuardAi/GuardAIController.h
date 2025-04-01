// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GuardAIController.generated.h"

class AGuardCharacter;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
struct FAIStimulus;

UCLASS()
class GAMEJAMPROJECT_API AGuardAIController : public AAIController
{
	GENERATED_BODY()

public:
	explicit AGuardAIController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	UAIPerceptionComponent* m_pPerceptionComponent{nullptr};

	UPROPERTY()
	UAISenseConfig_Hearing* m_pHearingConfig{nullptr};

	UPROPERTY()
	AGuardCharacter* m_ControlledEnemy{nullptr};
	
	void SetupHearingSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, const FAIStimulus stimulus);

	float m_CurrentSoundVolume{};
	FVector m_SoundLocation{};
	int32 m_HeardTimes{};
};
