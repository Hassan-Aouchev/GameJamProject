// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJamProject/GuardAi/BTTasks/BTTask_IncrementIndex.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameJamProject/GuardAi/GuardAIController.h"

UBTTask_IncrementIndex::UBTTask_IncrementIndex(const FObjectInitializer& ObjectInitializer):
	UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Increment Index and set value as false");
}

EBTNodeResult::Type UBTTask_IncrementIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(m_CurrentIndex.SelectedKeyName, OwnerComp.GetBlackboardComponent()->GetValueAsInt(m_CurrentIndex.SelectedKeyName) + 1);

	if (OwnerComp.GetBlackboardComponent()->GetValueAsInt(m_CurrentIndex.SelectedKeyName) >= m_MaxAmountIndex)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(m_CurrentIndex.SelectedKeyName, 0);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}