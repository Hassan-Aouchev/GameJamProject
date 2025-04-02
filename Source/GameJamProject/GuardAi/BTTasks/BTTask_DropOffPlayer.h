// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DropOffPlayer.generated.h"

UCLASS()
class GAMEJAMPROJECT_API UBTTask_DropOffPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_DropOffPlayer(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category="BlackBoard", meta=(AllowPrivateAccess=true))
	FBlackboardKeySelector m_RelevantPlayer;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	FBlackboardKeySelector m_IsPlayerPickedUp;
};
