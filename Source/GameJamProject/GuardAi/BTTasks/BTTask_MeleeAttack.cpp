// Fill out your copyright notice in the Description page of Project Settings.


#include "GameJamProject/GuardAi/BTTasks/BTTask_MeleeAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameJamProject/GuardAi/GuardCharacter.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack(const FObjectInitializer& ObjectInitializer):
UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* Object {OwnerComp.GetBlackboardComponent()->GetValueAsObject(m_PlayerToAttack.SelectedKeyName)};

	const AAIController* Controller {OwnerComp.GetAIOwner()};
	AGuardCharacter* EnemyCharacter {Cast<AGuardCharacter>(Controller->GetPawn())};
	//APlayerCharacter* Player {Cast<APlayerCharacter>(Object)};

	//if (EnemyCharacter->GetDistanceTo(Player) > m_MeleeRange)
	{
		return EBTNodeResult::Failed;
	}
	
	if (EnemyCharacter)
	{
		//if (Player)
		{
			//EnemyCharacter->MeeleAttack(Player, m_MeleeRange);
		}
	}

	return EBTNodeResult::Succeeded;
}

