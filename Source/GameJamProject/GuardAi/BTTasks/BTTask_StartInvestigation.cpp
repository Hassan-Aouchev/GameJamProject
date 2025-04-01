// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJamProject/GuardAi/BTTasks/BTTask_StartInvestigation.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_StartInvestigation::UBTTask_StartInvestigation(const FObjectInitializer& ObjectInitializer):
	UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Start Investigation");
}

EBTNodeResult::Type UBTTask_StartInvestigation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(m_IsInvestigating.SelectedKeyName, true);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(m_DisableBool.SelectedKeyName, false);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(m_SetLocationToInvestigate.SelectedKeyName, OwnerComp.GetBlackboardComponent()->GetValueAsVector(m_LocationToInvestigateWith.SelectedKeyName));
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
