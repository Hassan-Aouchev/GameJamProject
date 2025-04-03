#include "BTTask_DestroySelf.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "GameJamProject/GuardAi/GuardCharacter.h"

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
		AGuardCharacter* guard = Cast<AGuardCharacter>(AIPawn);
		if (guard)
		{
			guard->Satisfied();
			AIPawn->Destroy();
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}