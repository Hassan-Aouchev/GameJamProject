// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindPathPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameJamProject/GuardAi/GuardAIController.h"
#include "GameJamProject/GuardAi/GuardCharacter.h"
#include "GameJamProject/GuardAi/PathPoints.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint(const FObjectInitializer& ObjectInitializer):
	UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGuardAIController* Controller {Cast<AGuardAIController>(OwnerComp.GetAIOwner())};
	
	if (Controller)
	{
		UBlackboardComponent* BlackboardComponent {OwnerComp.GetBlackboardComponent()};
		
		if (BlackboardComponent)
		{
			const int32 Index{BlackboardComponent->GetValueAsInt(GetSelectedBlackboardKey())};

			AGuardCharacter* EnemyCharacter {Cast<AGuardCharacter>(Controller->GetPawn())};
			
			if (EnemyCharacter)
			{
				const FVector Point{EnemyCharacter->GetPatrolPath()->GetPathPoint(Index)};

				const UE::Math::TVector<double> GlobalPoint {EnemyCharacter->GetPatrolPath()->GetActorTransform().TransformPosition(Point)};
				BlackboardComponent->SetValueAsVector(m_PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
