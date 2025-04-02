#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DestroySelf.generated.h"

UCLASS()
class GAMEJAMPROJECT_API UBTTask_DestroySelf : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_DestroySelf();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};