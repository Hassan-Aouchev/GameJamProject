// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJamProject/GuardAi/BTTasks/BTTask_DropOffPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameJamProject/GhostCharacter.h"
#include "GameJamProject/GuardAi/GuardCharacter.h"

UBTTask_DropOffPlayer::UBTTask_DropOffPlayer(const FObjectInitializer& ObjectInitializer):
	UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Drop Off Player");
}

EBTNodeResult::Type UBTTask_DropOffPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* Object = OwnerComp.GetBlackboardComponent()->GetValueAsObject(m_RelevantPlayer.SelectedKeyName);

	const AAIController* Controller = OwnerComp.GetAIOwner();
	AGuardCharacter* EnemyCharacter = Cast<AGuardCharacter>(Controller->GetPawn());
	AGhostCharacter* Player = Cast<AGhostCharacter>(Object);

	if (EnemyCharacter)
	{
		if (Player)
		{
			Player->GhostReleased(EnemyCharacter);

			OwnerComp.GetBlackboardComponent()->SetValueAsBool(m_IsPlayerPickedUp.SelectedKeyName, false);
			
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
