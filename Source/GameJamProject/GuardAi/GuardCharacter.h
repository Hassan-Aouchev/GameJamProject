// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GuardCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStunned);

class AGuardAIController;
class AGhostCharacter;
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
	
	APathPoints* GetDropOffLocationHunter() const;

	UAnimMontage* GetAnimMontage() const;

	int32 GetFearLevel() const;
	void AddFearLevel(int amountToAdd);

	bool IsScared() const;

	bool GetSmoothOperator();
	void Stun();

	UPROPERTY(BlueprintAssignable)
	FOnStunned OnStunned;

	UFUNCTION(BlueprintCallable, Category = "Ghost")
	void SetPossessed(bool setValue);

	TArray<FName> GetFearNames() const;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta = (AllowPrivateAccess = "true"))
	APathPoints* m_pDropOffLocationHunter{nullptr};

	UPROPERTY()
	AGhostCharacter* m_Player{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta = (AllowPrivateAccess = "true"))
	float DetectionRadius = 1000.0f;
	
	int32 m_FearLevel{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="AI", meta = (AllowPrivateAccess = "true"))
	TArray<FName> Fears;
	
	bool FearHastStruck{};
	bool DoItOnce{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behaviour", meta = (AllowPrivateAccess = "true"))
	int32 m_MaxFearLevel{10};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behaviour", meta = (AllowPrivateAccess = "true"))
	bool m_SmoothOperator{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behaviour", meta = (AllowPrivateAccess = "true"))
	bool m_IsHunter{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behaviour", meta = (AllowPrivateAccess = "true"))
	float m_Vicinity{300.f};

	FTimerHandle FearCooldownTimerHandle;
	bool bCanSpreadFear = true;
	bool bHasSeenPlayer{};

	UPROPERTY()
	AGuardAIController* AIController{nullptr};
	
	UPROPERTY()
	UCapsuleComponent* m_SpreadFearCapsule{nullptr};
	
	UPROPERTY()
	UCapsuleComponent* m_PlayerDetectedCapsule{nullptr};

	UFUNCTION()
	void OnCapsuleOverlapFear(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnCapsuleOverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEndPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ResetFearCooldown();

	bool bPossessed{};

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};
