// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementPathIndex.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameJamProject/GuardAi/GuardAIController.h"
#include "GameJamProject/GuardAi/GuardCharacter.h"
#include "GameJamProject/GuardAi/PathPoints.h"

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(const FObjectInitializer& ObjectInitializer):
	UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGuardAIController* Controller {Cast<AGuardAIController>(OwnerComp.GetAIOwner())};
	
	if (Controller)
	{
		AGuardCharacter* EnemyCharacter {Cast<AGuardCharacter>(Controller->GetPawn())};
		
		if (EnemyCharacter)
		{
			UBlackboardComponent* BlackboardComponent {OwnerComp.GetBlackboardComponent()};
			
			if (BlackboardComponent)
			{
				const int NoOfPoints{EnemyCharacter->GetPatrolPath()->Num()};
				const int MinIndex{};
				const int MaxIndex{NoOfPoints - 1};
				int Index{BlackboardComponent->GetValueAsInt(GetSelectedBlackboardKey())};

				if (m_Bidirectional)
				{
					if (Index >= MaxIndex && m_Direction == EDirectionType::Forward)
					{
						m_Direction = EDirectionType::Backwards;
					}
					else if (Index == MinIndex && m_Direction == EDirectionType::Backwards)
					{
						m_Direction = EDirectionType::Forward;
					}
				}

				BlackboardComponent->SetValueAsInt(GetSelectedBlackboardKey(), (m_Direction == EDirectionType::Forward ? ++Index : --Index) % NoOfPoints);
				
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
