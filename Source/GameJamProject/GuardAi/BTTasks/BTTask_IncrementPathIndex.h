// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_IncrementPathIndex.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMPROJECT_API UBTTask_IncrementPathIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UBTTask_IncrementPathIndex(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	enum class EDirectionType	{ Forward, Backwards };

	EDirectionType m_Direction	{EDirectionType::Forward};
	
	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "behavior", Meta = (AllowPrivateAccess="true"))
	bool m_Bidirectional{false};
};
