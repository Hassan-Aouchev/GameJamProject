// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GuardCharacter.generated.h"

class APlayerCharacter;
class APathPoints;
class UBehaviorTree;

UCLASS()
class GAMEJAMPROJECT_API AGuardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGuardCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const;

	APathPoints* GetPatrolPath() const;

	APathPoints* GetDropOffLocation() const;

	UAnimMontage* GetAnimMontage() const;

	int32 GetFearLevel() const;
	void AddFearLevel(int amountToAdd);

	bool IsScared() const;

	bool GetSmoothOperator();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* m_pBehaviorTree{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta = (AllowPrivateAccess = "true"))
	APathPoints* m_pPatrolPath{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* m_pAnimMontage{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta = (AllowPrivateAccess = "true"))
	APathPoints* m_pDropOffLocation{nullptr};

	int32 m_FearLevel{};

	bool FearHastStruck{};
	bool DoItOnce{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behaviour", meta = (AllowPrivateAccess = "true"))
	int32 m_MaxFearLevel{10};

	int32 m_HeardAmount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behaviour", meta = (AllowPrivateAccess = "true"))
	bool m_SmoothOperator{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behaviour", meta = (AllowPrivateAccess = "true"))
	float m_Vicinity{300.f};

	FTimerHandle FearCooldownTimerHandle;
	bool bCanSpreadFear = true;
	
	UPROPERTY()
	UCapsuleComponent* m_SpreadFearCapsule{nullptr};

	UFUNCTION()
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);

	void ResetFearCooldown();

};
