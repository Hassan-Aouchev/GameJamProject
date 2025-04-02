// Fill out your copyright notice in the Description page of Project Settings.


#include "GameJamProject/GuardAi/BTTasks/BTTask_MeleeAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameJamProject/GhostCharacter.h"
#include "GameJamProject/GuardAi/GuardCharacter.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack(const FObjectInitializer& ObjectInitializer):
UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Melee Attack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* Object {OwnerComp.GetBlackboardComponent()->GetValueAsObject(m_PlayerToAttack.SelectedKeyName)};

	AAIController* Controller {OwnerComp.GetAIOwner()};
	AGuardCharacter* EnemyCharacter {Cast<AGuardCharacter>(Controller->GetPawn())};
	AGhostCharacter* Player {Cast<AGhostCharacter>(Object)};

	if (!EnemyCharacter || !Player)
	{
		return EBTNodeResult::Failed;
	}

	if (EnemyCharacter->GetDistanceTo(Player) > m_MeleeRange)
	{
		return EBTNodeResult::Failed;
	}

	// Start the tug-of-war
	Player->StartTugOfWar(EnemyCharacter);
    
	// Task is still running
	return EBTNodeResult::InProgress;
}

void UBTTask_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UObject* Object {OwnerComp.GetBlackboardComponent()->GetValueAsObject(m_PlayerToAttack.SelectedKeyName)};
    
	AGhostCharacter* Player {Cast<AGhostCharacter>(Object)};

	if(!Player->GetIsBeingCaptured())
	{
		if (!Player->GetHasWon())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
}