#include "BTTask_DestroySelf.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

UBTTask_DestroySelf::UBTTask_DestroySelf()
{
	NodeName = "Destroy Self";
}

EBTNodeResult::Type UBTTask_DestroySelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* AIPawn = AIController->GetPawn();
	if (AIPawn)
	{
		AIPawn->Destroy();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}