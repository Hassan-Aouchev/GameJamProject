#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetVector.generated.h"

UCLASS()
class GAMEJAMPROJECT_API UBTTask_SetVector : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_SetVector(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	/** The blackboard key that will be set */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetVectorKey;

};