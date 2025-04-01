// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetBool.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMPROJECT_API UBTTask_SetBool : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UBTTask_SetBool(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category="BlackBoard", meta=(AllowPrivateAccess=true))
	FBlackboardKeySelector m_BoolValueToSet;

	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool m_BoolValue{};
};
