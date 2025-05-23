// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_StartInvestigation.generated.h"

UCLASS()
class GAMEJAMPROJECT_API UBTTask_StartInvestigation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_StartInvestigation(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category="BlackBoard", meta=(AllowPrivateAccess=true))
	FBlackboardKeySelector m_IsInvestigating;

	UPROPERTY(EditAnywhere, Category="BlackBoard", meta=(AllowPrivateAccess=true))
	FBlackboardKeySelector m_DisableBool;

	UPROPERTY(EditAnywhere, Category="BlackBoard", meta=(AllowPrivateAccess=true))
	FBlackboardKeySelector m_SetLocationToInvestigate;

	UPROPERTY(EditAnywhere, Category="BlackBoard", meta=(AllowPrivateAccess=true))
	FBlackboardKeySelector m_LocationToInvestigateWith;
};
