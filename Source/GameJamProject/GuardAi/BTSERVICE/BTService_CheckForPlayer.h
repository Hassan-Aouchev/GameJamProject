#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckForPlayer.generated.h"

UCLASS()
class GAMEJAMPROJECT_API UBTService_CheckForPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckForPlayer();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "AI")
	float DetectionRadius = 1000.0f;
	
	UPROPERTY(EditAnywhere, Category="BlackBoard", meta=(AllowPrivateAccess=true))
	FBlackboardKeySelector m_CanSeePlayer;
};
