// Fill out your copyright notice in the Description page of Project Settings.


#include "GameJamProject/GuardAi/BTTasks/BTTask_FindDropOffPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameJamProject/GuardAi/GuardAIController.h"
#include "GameJamProject/GuardAi/GuardCharacter.h"
#include "GameJamProject/GuardAi/PathPoints.h"

UBTTask_FindDropOffPoint::UBTTask_FindDropOffPoint(const FObjectInitializer& ObjectInitializer):
	UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Find Drop Off Point");
}

EBTNodeResult::Type UBTTask_FindDropOffPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGuardAIController* Controller {Cast<AGuardAIController>(OwnerComp.GetAIOwner())};
	
	if (Controller)
	{
		UBlackboardComponent* BlackboardComponent {OwnerComp.GetBlackboardComponent()};
		
		if (BlackboardComponent)
		{
			AGuardCharacter* EnemyCharacter {Cast<AGuardCharacter>(Controller->GetPawn())};
			
			if (EnemyCharacter)
			{
				const FVector Point		{EnemyCharacter->GetDropOffLocation()->GetPathPoint(0)};

				const UE::Math::TVector<double> GlobalPoint		{EnemyCharacter->GetDropOffLocation()->GetActorTransform().TransformPosition(Point)};
				
				BlackboardComponent->SetValueAsVector(m_DropOffPathVectorKey.SelectedKeyName, GlobalPoint);
				
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
