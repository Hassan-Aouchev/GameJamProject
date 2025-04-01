// Fill out your copyright notice in the Description page of Project Settings.


#include "GameJamProject/GuardAi/BTTasks/BTTask_SetBool.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetBool::UBTTask_SetBool(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Set Bool");
}

EBTNodeResult::Type UBTTask_SetBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(m_BoolValueToSet.SelectedKeyName, m_BoolValue);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
