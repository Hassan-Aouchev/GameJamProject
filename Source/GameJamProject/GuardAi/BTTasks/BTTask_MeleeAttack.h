// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MeleeAttack.generated.h"

class AGuardCharacter;
/**
 * 
 */
UCLASS()
class GAMEJAMPROJECT_API UBTTask_MeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_MeleeAttack(const FObjectInitializer& ObjectInitializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category="BlackBoard", meta=(AllowPrivateAccess=true))
	FBlackboardKeySelector m_PlayerToAttack;
	
	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float m_MeleeRange{2.5f};
};
