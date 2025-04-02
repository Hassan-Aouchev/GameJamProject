// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameJamProject/GuardAi/GuardAIController.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(const FObjectInitializer& ObjectInitializer):
	UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AGuardAIController* EnemyAIController {Cast<AGuardAIController>(OwnerComp.GetAIOwner())};
	
	if (EnemyAIController)
	{
		const UNavigationSystemV1* NavSys {UNavigationSystemV1::GetCurrent(GetWorld())};

		FVector OriginalPoint{OwnerComp.GetBlackboardComponent()->GetValueAsVector(m_SearchLocation.SelectedKeyName)}; 
		
		if (NavSys)
		{
			FNavLocation SearchLocation{};
			if (NavSys->GetRandomPointInNavigableRadius(OriginalPoint, m_SearchRadius, SearchLocation))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), SearchLocation.Location);
			}
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
