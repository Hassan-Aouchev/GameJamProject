// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameJamProject/GuardAi/GuardAIController.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(const FObjectInitializer& ObjectInitializer):
	UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGuardAIController* Controller {Cast<AGuardAIController>(OwnerComp.GetAIOwner())};
	
	if (Controller)
	{
		const FVector PlayerLocation{OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey())};

		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, PlayerLocation);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
