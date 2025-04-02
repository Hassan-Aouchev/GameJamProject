// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMPROJECT_API UBTTask_FindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_FindRandomLocation(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Search",meta=(AllowPrivateAccess=true))
	float m_SearchRadius{1500.f};

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Search",meta=(AllowPrivateAccess=true))
	FBlackboardKeySelector m_SearchLocation;
};
