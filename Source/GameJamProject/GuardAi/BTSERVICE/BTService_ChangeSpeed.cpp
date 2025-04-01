// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_ChangeSpeed.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameJamProject/GuardAi/GuardCharacter.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	const AAIController* Controller {OwnerComp.GetAIOwner()};
	
	if (Controller)
	{
		const AGuardCharacter* EnemyCharacter {Cast<AGuardCharacter>(Controller->GetPawn())};
		
		if (EnemyCharacter)
		{
			EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = m_Speed;
		}
	}
}
