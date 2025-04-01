#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_LookAtLocation.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMPROJECT_API UBTTask_LookAtLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UBTTask_LookAtLocation(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "AI")
	float RotationSpeed = 2.0f;
};
