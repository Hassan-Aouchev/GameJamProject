#include "BTTask_SetVector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameJamProject/GuardAi/GuardAIController.h"
#include "GameJamProject/GuardAi/GuardCharacter.h"

UBTTask_SetVector::UBTTask_SetVector(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Set Vector");
}

EBTNodeResult::Type UBTTask_SetVector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return EBTNodeResult::Failed;
	}

	AGuardAIController* Controller {Cast<AGuardAIController>(OwnerComp.GetAIOwner())};

	if(Controller)
	{
		AGuardCharacter* EnemyCharacter {Cast<AGuardCharacter>(Controller->GetPawn())};

		if(EnemyCharacter)
		{
			BlackboardComponent->SetValueAsVector(TargetVectorKey.SelectedKeyName,EnemyCharacter->GetActorLocation());
		}
	}

	return EBTNodeResult::Succeeded;
}
